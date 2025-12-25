#include "views/GameView.h"
#include "controllers/GameController.h"

GameView::GameView()
    : _playfieldView(nullptr)
    , _baseStackView(nullptr)
    , _reserveStackView(nullptr)
    , _undoMenuItem(nullptr)
	, _undoButtonBg(nullptr)
    , _controller(nullptr) {
}

GameView::~GameView() {
    CC_SAFE_DELETE(_controller);
}

GameView* GameView::create() {
    GameView* ret = new (std::nothrow) GameView();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameView::init() {
    if (!Scene::init()) {
        return false;
    }

    CCLOG("========== GameView:: init ==========");

    // 初始化UI
    initUI();

    // 创建控制器
    _controller = new (std::nothrow) GameController();
    if (!_controller) {
        CCLOG("ERROR:  Failed to create GameController");
        return false;
    }

    if (!_controller->init(this)) {
        CCLOG("ERROR: Failed to init GameController");
        return false;
    }

    // 设置回调
    setOnCardClickCallback([this](int cardId) {
        CCLOG("GameView: Card clicked, id=%d", cardId);
        if (_controller) {
            _controller->onCardClicked(cardId);
        }
        });

    setOnUndoClickCallback([this]() {
        CCLOG("GameView: Undo button clicked");
        if (_controller) {
            _controller->onUndoClicked();
        }
        });

    // 启动游戏
    _controller->startGame(1);

    CCLOG("GameView initialized successfully");
    CCLOG("====================================");

    return true;
}

void GameView::initUI() {
    // 设置背景色
    /*auto bg = LayerColor::create(Color4B(34, 139, 34, 255));
    this->addChild(bg, -100);*/
    

    // 上半部分桌面牌区域背景
    auto topBg = LayerColor::create(Color4B(170, 121, 66, 255));
    topBg->setContentSize(Size(1080, 1500));
    topBg->setPosition(Vec2(0, 580));
    this->addChild(topBg, -50);
    CCLOG("GameView: Top background created");


    // 创建桌面牌区（位置：屏幕上方）
    _playfieldView = PlayfieldView::create();
    if (_playfieldView) {
        _playfieldView->setPosition(Vec2(0, 580));
        this->addChild(_playfieldView, 1);
        CCLOG("GameView: PlayfieldView created at (0, 580)");
    }

    // 下半部分手牌区域背景
    auto bottomBg = LayerColor::create(Color4B(148, 33, 146, 255));
    bottomBg->setContentSize(Size(1080, 580));
    bottomBg->setPosition(Vec2(0, 0));
    this->addChild(bottomBg, -50);
    CCLOG("GameView: Bottom background created");

    // 创建手牌区（位置：屏幕下方中间）
    _baseStackView = StackView::create();
    if (_baseStackView) {
        _baseStackView->setPosition(Vec2(540, 200));
        this->addChild(_baseStackView, 5);
        CCLOG("GameView: BaseStackView created at (540, 200)");
    }

    // 创建备用牌堆（位置：屏幕下方左侧）
    _reserveStackView = StackView::create();
    if (_reserveStackView) {
        _reserveStackView->setPosition(Vec2(200, 200));
        this->addChild(_reserveStackView, 10);
        CCLOG("GameView: ReserveStackView created at (200, 200)");
    }
    // 按钮背景（灰色矩形）
    _undoButtonBg = LayerColor::create(Color4B(150, 150, 150, 255));  // 初始灰色
    _undoButtonBg->setContentSize(Size(120, 60));
    _undoButtonBg->setPosition(Vec2(820, 170));
    this->addChild(_undoButtonBg, 19);

    // 文字标签微软雅黑粗体
    auto undoLabel = Label::createWithSystemFont("回退", "Arial", 50);  // 加粗
    undoLabel->setTextColor(Color4B(200, 200, 200, 255));  // 初始浅灰色

    // 菜单项
    _undoMenuItem = MenuItemLabel::create(undoLabel, [this](Ref*) {
        CCLOG("GameView: Undo button clicked");
        if (_onUndoClickCallback) {
            _onUndoClickCallback();
        }
        });

    // 菜单
    auto menu = Menu::create(_undoMenuItem, nullptr);
    menu->setPosition(Vec2(880, 200));
    this->addChild(menu, 20);

    // 初始禁用
    _undoMenuItem->setEnabled(false);

    CCLOG("GameView: Simple undo button created");
    CCLOG("======================================");
}

bool GameView::createCardsFromModel(const GameModel& gameModel) {
    CCLOG("========== GameView::createCardsFromModel ==========");

    // 清空现有卡牌
    if (_playfieldView) _playfieldView->clear();
    if (_baseStackView) _baseStackView->clear();
    if (_reserveStackView) _reserveStackView->clear();

    // 创建桌面牌区的卡牌
    for (const auto& cardModel : gameModel.getPlayfield()) {
        auto cardView = CardView::create(cardModel.face, cardModel.suit, cardModel.isFaceUp);
        if (cardView) {
            cardView->setCardId(cardModel.id);
            cardView->setPosition(Vec2(cardModel.posX, cardModel.posY));
            cardView->setOnClickCallback(_onCardClickCallback);
            _playfieldView->addCard(cardView);
            CCLOG("  Created playfield card: id=%d, pos=(%.1f, %.1f)",
                cardModel.id, cardModel.posX, cardModel.posY);
        }
    }

    // 创建手牌区的卡牌
    for (const auto& cardModel : gameModel.getBaseStack()) {
        auto cardView = CardView::create(cardModel.face, cardModel.suit, cardModel.isFaceUp);
        if (cardView) {
            cardView->setCardId(cardModel.id);
            cardView->setPosition(Vec2(0, 0));  // 堆叠在一起
            cardView->setOnClickCallback(_onCardClickCallback);
            _baseStackView->addCard(cardView);
            CCLOG("  Created base stack card: id=%d", cardModel.id);
        }
    }

    // 创建备用牌堆的卡牌
    for (const auto& cardModel : gameModel.getReserveStack()) {
        auto cardView = CardView::create(cardModel.face, cardModel.suit, cardModel.isFaceUp);
        if (cardView) {
            cardView->setCardId(cardModel.id);
            cardView->setPosition(Vec2(0, 0));  // 堆叠在一起
            cardView->setOnClickCallback(_onCardClickCallback);
            _reserveStackView->addCard(cardView);
            CCLOG("  Created reserve stack card: id=%d", cardModel.id);
        }
    }

    CCLOG("Cards created successfully");
    CCLOG("====================================================");

    return true;
}

void GameView::playMatchAnimation(int cardId, Vec2 targetPos, std::function<void()> callback) {
    CCLOG("GameView: Playing match animation for card %d", cardId);

    CardView* cardView = findCardViewById(cardId);
    if (!cardView) {
        CCLOG("ERROR: Card view not found");
        if (callback) callback();
        return;
    }

    // 转换坐标
    Vec2 worldPos = _baseStackView->convertToWorldSpace(targetPos);
    Vec2 localPos = _playfieldView->convertToNodeSpace(worldPos);

    // 播放移动动画
    cardView->playMoveAnimation(localPos, [this, cardView, callback]() {
        // 动画完成后，从桌面牌区移除，添加到手牌区
        _playfieldView->removeCard(cardView);

        cardView->retain();
        cardView->setPosition(Vec2(0, 0));
        _baseStackView->addCard(cardView);
        cardView->release();

        if (callback) {
            callback();
        }
        });
}

void GameView::playDrawFromReserveAnimation(int cardId, Vec2 targetPos, std::function<void()> callback) {
    CCLOG("GameView: Playing draw from reserve animation for card %d", cardId);

    CardView* cardView = findCardViewById(cardId);
    if (!cardView) {
        CCLOG("ERROR: Card view not found");
        if (callback) callback();
        return;
    }

    // 转换坐标
    Vec2 worldPos = _baseStackView->convertToWorldSpace(targetPos);
    Vec2 localPos = _reserveStackView->convertToNodeSpace(worldPos);

    // 播放移动动画
    cardView->playMoveAnimation(localPos, [this, cardView, callback]() {
        // 动画完成后，从备用牌堆移除，添加到手牌区
        _reserveStackView->removeCard(cardView);

        cardView->retain();
        cardView->setPosition(Vec2(0, 0));
        _baseStackView->addCard(cardView);
        cardView->release();

        if (callback) {
            callback();
        }
        });
}

void GameView::playUndoAnimation(int cardId, Vec2 targetPos, std::function<void()> callback) {
    CCLOG("GameView: Playing undo animation for card %d", cardId);

    CardView* cardView = findCardViewById(cardId);
    if (!cardView) {
        CCLOG("ERROR: Card view not found");
        if (callback) callback();
        return;
    }
    // 先retain保持对象存活,再从手牌区移除
    cardView->retain();

    // 从手牌区移除
    _baseStackView->removeCard(cardView);

    // 根据目标位置判断要移动到哪个区域
    // 简化处理：如果 y > 500，移到桌面牌区，否则移到备用牌堆
    bool toPlayfield = targetPos.y > 500;

    if (toPlayfield) {
        cardView->retain();
        cardView->setPosition(targetPos);
        _playfieldView->addCard(cardView);
        cardView->release();

        if (callback) callback();
    }
    else {
        cardView->retain();
        cardView->setPosition(Vec2(0, 0));
        _reserveStackView->addCard(cardView);
        cardView->release();

        if (callback) callback();
    }
}

void GameView::playCardShakeAnimation(int cardId) {
    CardView* cardView = findCardViewById(cardId);
    if (cardView) {
        cardView->playShakeAnimation();
    }
}

void GameView::showVictoryDialog() {
    CCLOG("GameView:  Showing victory dialog");

    // 简单的胜利提示
    auto label = Label::createWithSystemFont("Victory!", "Arial", 100);
    label->setPosition(Vec2(540, 1040));
    label->setTextColor(Color4B::YELLOW);
    this->addChild(label, 100);

    // 淡入效果
    label->setOpacity(0);
    label->runAction(FadeIn::create(0.5f));
}

void GameView::setUndoButtonEnabled(bool enabled) {
    if (_undoMenuItem) {
        _undoMenuItem->setEnabled(enabled);

        // 获取文字标签
        auto label = dynamic_cast<Label*>(_undoMenuItem->getLabel());

        if (enabled) {
            // 启用状态：亮色
            if (label) {
                label->setTextColor(Color4B::WHITE);
            }
            if (_undoButtonBg) {
                _undoButtonBg->setColor(Color3B(80, 130, 200));  // 蓝色
            }
        }
        else {
            // 禁用状态：灰色
            if (label) {
                label->setTextColor(Color4B(200, 200, 200, 255));  // 浅灰色文字
            }
            if (_undoButtonBg) {
                _undoButtonBg->setColor(Color3B(150, 150, 150));  // 深灰色背景
            }
        }

        CCLOG("GameView: Undo button %s", enabled ? "enabled" : "disabled");
    }
}

void GameView::setOnCardClickCallback(CardClickCallback callback) {
    _onCardClickCallback = callback;
}

void GameView::setOnUndoClickCallback(UndoClickCallback callback) {
    _onUndoClickCallback = callback;
}

CardView* GameView::findCardViewById(int cardId) {
    // 先在桌面牌区找
    CardView* card = _playfieldView->getCardById(cardId);
    if (card) return card;

    // 再在手牌区找
    card = _baseStackView->getCardById(cardId);
    if (card) return card;

    // 最后在备用牌堆找
    card = _reserveStackView->getCardById(cardId);
    return card;
}