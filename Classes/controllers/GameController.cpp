#include "controllers/GameController.h"
#include "views/GameView.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelFromLevelGenerator.h"
#include "utils/AnimationUtils.h"

GameController::GameController()
    : _gameModel(nullptr)
    , _gameView(nullptr)
    , _undoManager(nullptr) {
}

GameController::~GameController() {
    CC_SAFE_DELETE(_gameModel);
    CC_SAFE_DELETE(_undoManager);
}

bool GameController::init(GameView* gameView) {
    CCLOG("========== GameController:: init ==========");

    if (!gameView) {
        CCLOG("ERROR: gameView is null");
        return false;
    }

    _gameView = gameView;

    // 创建游戏模型
    _gameModel = new (std::nothrow) GameModel();
    if (!_gameModel) {
        CCLOG("ERROR: Failed to create GameModel");
        return false;
    }

    // 创建回退管理器
    _undoManager = new (std::nothrow) UndoManager();
    if (!_undoManager) {
        CCLOG("ERROR: Failed to create UndoManager");
        return false;
    }
    _undoManager->init(100);  // 最多100步回退

    CCLOG("GameController initialized successfully");
    return true;
}

bool GameController::startGame(int levelId) {
    CCLOG("========== GameController::startGame ==========");
    CCLOG("Level ID: %d", levelId);

    // 加载关卡配置
    std::string filename = "level1.json"; 
    LevelConfig config = LevelConfigLoader::loadFromFile(filename);

    if (config.playfieldCards.empty() && config.stackCards.empty()) {
        CCLOG("ERROR: Failed to load level config");
        return false;
    }

    // 生成游戏模型
    if (!GameModelFromLevelGenerator::generateFromConfig(config, *_gameModel)) {
        CCLOG("ERROR: Failed to generate game model");
        return false;
    }

    // 清空回退记录
    _undoManager->clear();

    // 创建视图
    if (!_gameView->createCardsFromModel(*_gameModel)) {
        CCLOG("ERROR: Failed to create cards view");
        return false;
    }

    // 更新UI
    updateUndoButton();

    CCLOG("Game started successfully");
    CCLOG("==========================================");

    return true;
}

void GameController::onCardClicked(int cardId) {
    CCLOG("========== GameController::onCardClicked ==========");
    CCLOG("Card ID: %d", cardId);

    // 查找卡牌
    CardModel card = _gameModel->getCardById(cardId);
    if (card.id == -1) {
        CCLOG("ERROR:  Card not found");
        return;
    }

    // 判断卡牌所在区域
    if (_gameModel->isCardInPlayfield(cardId)) {
        handlePlayfieldCardClick(cardId);
    }
    else if (_gameModel->isCardInReserveStack(cardId)) {
        handleReserveStackClick(cardId);
    }
    else {
        CCLOG("Card is in base stack, cannot click");
    }

    CCLOG("===================================================");
}

void GameController::handlePlayfieldCardClick(int cardId) {
    CCLOG("Handling playfield card click: id=%d", cardId);

    // 获取手牌区顶牌
    CardModel baseTop = _gameModel->getBaseStackTop();
    if (baseTop.id == -1) {
        CCLOG("ERROR: Base stack is empty");
        return;
    }

    // 获取被点击的卡牌
    CardModel clickedCard = _gameModel->getCardById(cardId);

    // 检查是否可以匹配
    if (!canMatch(clickedCard, baseTop)) {
        CCLOG("Cannot match:  clicked face=%d, base face=%d", clickedCard.face, baseTop.face);
        _gameView->playCardShakeAnimation(cardId);  // 播放抖动动画
        return;
    }

    CCLOG("Cards matched!  Moving card %d to base stack", cardId);

    // 记录回退信息
    UndoRecord record;
    record.actionType = UndoActionType::MOVE_CARD;
    record.cardId = cardId;
    record.fromArea = CardArea::PLAYFIELD;
    record.toArea = CardArea::BASE_STACK;
    record.fromPos = Vec2(clickedCard.posX, clickedCard.posY);
    record.toPos = Vec2(baseTop.posX, baseTop.posY);
    _undoManager->pushRecord(record);

    // 更新 Model
    _gameModel->removeCardFromPlayfield(cardId);
    clickedCard.posX = baseTop.posX;
    clickedCard.posY = baseTop.posY;
    _gameModel->addCardToBaseStack(clickedCard);

    // 更新 View（播放动画）
    _gameView->playMatchAnimation(cardId, Vec2(baseTop.posX, baseTop.posY), [this]() {
        // 动画完成后检查胜利
        if (checkVictory()) {
            _gameView->showVictoryDialog();
        }
        updateUndoButton();
        });
}

void GameController::handleReserveStackClick(int cardId) {
    CCLOG("Handling reserve stack click: id=%d", cardId);

    // 获取备用牌堆的顶牌（最后一张）
    const auto& reserveStack = _gameModel->getReserveStack();
    if (reserveStack.empty()) {
        CCLOG("Reserve stack is empty");
        return;
    }

    CardModel topCard = reserveStack.back();
    if (topCard.id != cardId) {
        CCLOG("Can only click top card of reserve stack");
        return;
    }

    // 获取当前手牌区顶牌
    CardModel baseTop = _gameModel->getBaseStackTop();

    // 记录回退信息
    UndoRecord record;
    record.actionType = UndoActionType::DRAW_FROM_RESERVE;
    record.cardId = cardId;
    record.fromArea = CardArea::RESERVE_STACK;
    record.toArea = CardArea::BASE_STACK;
    record.fromPos = Vec2(topCard.posX, topCard.posY);
    record.toPos = Vec2(baseTop.posX, baseTop.posY);
    _undoManager->pushRecord(record);

    // 更新 Model
    _gameModel->removeCardFromReserveStack(cardId);
    topCard.posX = baseTop.posX;
    topCard.posY = baseTop.posY;
    _gameModel->addCardToBaseStack(topCard);

    // 更新 View（播放动画）
    _gameView->playDrawFromReserveAnimation(cardId, Vec2(baseTop.posX, baseTop.posY), [this]() {
        updateUndoButton();
        });
}

void GameController::onUndoClicked() {
    CCLOG("========== GameController::onUndoClicked ==========");

    if (!_undoManager->canUndo()) {
        CCLOG("Cannot undo: no records");
        return;
    }

    // 弹出回退记录
    UndoRecord record = _undoManager->popRecord();

    // 执行回退
    executeUndo(record);

    CCLOG("===================================================");
}

void GameController::executeUndo(const UndoRecord& record) {
    CCLOG("Executing undo: cardId=%d, type=%d", record.cardId, static_cast<int>(record.actionType));

    // 从目标区域移除卡牌
    CardModel card = _gameModel->getCardById(record.cardId);

    if (record.toArea == CardArea::BASE_STACK) {
        _gameModel->removeCardFromBaseStack(record.cardId);
    }

    // 恢复到源区域
    card.posX = record.fromPos.x;
    card.posY = record.fromPos.y;

    if (record.fromArea == CardArea::PLAYFIELD) {
        _gameModel->addCardToPlayfield(card);
    }
    else if (record.fromArea == CardArea::RESERVE_STACK) {
        _gameModel->addCardToReserveStack(card);
    }

    // 播放回退动画
    _gameView->playUndoAnimation(record.cardId, record.fromPos, [this]() {
        updateUndoButton();
        });
}

bool GameController::canMatch(const CardModel& card1, const CardModel& card2) const {
    // 检查点数是否相差1
    int diff = std::abs(card1.face - card2.face);

    // 处理A和K的特殊情况（A=0, K=12）
    if ((card1.face == CFT_ACE && card2.face == CFT_KING) ||
        (card1.face == CFT_KING && card2.face == CFT_ACE)) {
        return true;
    }

    return diff == 1;
}

bool GameController::checkVictory() const {
    // 桌面牌区清空即为胜利
    return _gameModel->getPlayfield().empty();
}

void GameController::updateUndoButton() {
    bool canUndo = _undoManager->canUndo();
    _gameView->setUndoButtonEnabled(canUndo);
    CCLOG("Undo button updated: %s", canUndo ? "enabled" : "disabled");
}