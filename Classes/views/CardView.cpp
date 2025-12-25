#include "views/CardView.h"

CardView::CardView()
    : _cardId(-1)
    , _cardFace(0)
    , _cardSuit(0)
    , _isFaceUp(true)
    , _useBigCard(true)
    , _bgSprite(nullptr)
    , _suitSprite(nullptr)
    , _numberSprite(nullptr)
    , _touchEnabled(true) {
}

CardView:: ~CardView() {
}

CardView* CardView::create(int cardFace, int cardSuit, bool isFaceUp, bool useBigCard) {
    CardView* ret = new (std::nothrow) CardView();
    if (ret && ret->init(cardFace, cardSuit, isFaceUp, useBigCard)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CardView::init(int cardFace, int cardSuit, bool isFaceUp, bool useBigCard) {
    if (!Node::init()) {
        return false;
    }

    _cardFace = cardFace;
    _cardSuit = cardSuit;
    _isFaceUp = isFaceUp;
    _useBigCard = useBigCard;
    _cardId = -1;

    // 创建卡牌底图
    _bgSprite = Sprite::create("res/card_general.png");
    if (!_bgSprite) {
        CCLOG("ERROR: Failed to load card_general.png");
        return false;
    }
    _bgSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->addChild(_bgSprite);
    CCLOG("CardView:  Loaded card_general.png");

    // 创建花色图标
    std::string suitPath = getSuitImagePath(cardSuit);
    _suitSprite = Sprite::create(suitPath);
    if (!_suitSprite) {
        CCLOG("ERROR: Failed to load %s", suitPath.c_str());
        return false;
    }
    _suitSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _suitSprite->setPosition(Vec2(33, 57));  // 左上角位置
    _bgSprite->addChild(_suitSprite);
    CCLOG("CardView: Loaded %s", suitPath.c_str());

    // 创建点数图片
    std::string numberPath = getNumberImagePath(cardFace, cardSuit, useBigCard);
    _numberSprite = Sprite::create(numberPath);
    if (!_numberSprite) {
        CCLOG("ERROR: Failed to load %s", numberPath.c_str());
        return false;
    }
    _numberSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    _numberSprite->setPosition(Vec2(22, 26));  // 左上角位置
    _bgSprite->addChild(_numberSprite);
    CCLOG("CardView:  Loaded %s", numberPath.c_str());

    // 设置缩放
    float targetCardWidth = 150.0f;
    float scale = targetCardWidth / _bgSprite->getContentSize().width;
    _bgSprite->setScale(scale);

    // 设置内容大小
    Size scaledSize = _bgSprite->getContentSize() * scale;
    this->setContentSize(scaledSize);

    // 设置触摸监听
    setupTouchListener();

    // 设置初始显示状态
    setFaceUp(isFaceUp);

    CCLOG("CardView initialized:  face=%d, suit=%d, id=%d", _cardFace, _cardSuit, _cardId);

    return true;
}

void CardView::setupTouchListener() {
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        if (!_touchEnabled || !this->isVisible()) {
            return false;
        }

        Vec2 worldPos = touch->getLocation();
        Vec2 locationInNode = this->convertToNodeSpace(worldPos);

        Size s = _bgSprite->getContentSize() * _bgSprite->getScale();
        Rect rect(-s.width / 2, -s.height / 2, s.width, s.height);

        if (rect.containsPoint(locationInNode)) {
            CCLOG("CardView: Touch began on card id=%d at world(%.1f, %.1f), local(%.1f, %.1f)",
                _cardId, worldPos.x, worldPos.y, locationInNode.x, locationInNode.y);
            return true;
        }
        return false;
        };

    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        CCLOG("CardView: Card clicked, id=%d", _cardId);
        onCardClicked();
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CardView::onCardClicked() {
    if (_onClickCallback) {
        CCLOG("CardView: Calling callback for card id=%d", _cardId);
        _onClickCallback(_cardId);
    }
}

std::string CardView::getSuitImagePath(int cardSuit) const {
    switch (cardSuit) {
    case 0: return "res/suits/club.png";      // 梅花
    case 1: return "res/suits/diamond.png";   // 方块
    case 2: return "res/suits/heart.png";     // 红桃
    case 3: return "res/suits/spade.png";     // 黑桃
    default: return "res/suits/club.png";
    }
}

std::string CardView::getNumberImagePath(int cardFace, int cardSuit, bool useBig) const {
    // 颜色：方块1和红桃2为红色，梅花(0)和黑桃(3)为黑色
    std::string color = (cardSuit == 1 || cardSuit == 2) ? "red" : "black";

    // 大小前缀
    std::string sizePrefix = useBig ? "big" : "small";

    // 点数字符串
    std::string faceStr;
    switch (cardFace) {
    case 0:  faceStr = "A"; break;   // A
    case 1:  faceStr = "2"; break;   // 2
    case 2:  faceStr = "3"; break;   // 3
    case 3:  faceStr = "4"; break;   // 4
    case 4:  faceStr = "5"; break;   // 5
    case 5:  faceStr = "6"; break;   // 6
    case 6:  faceStr = "7"; break;   // 7
    case 7:  faceStr = "8"; break;   // 8
    case 8:  faceStr = "9"; break;   // 9
    case 9:  faceStr = "10"; break;  // 10
    case 10: faceStr = "J"; break;   // J
    case 11: faceStr = "Q"; break;   // Q
    case 12: faceStr = "K"; break;   // K
    default: faceStr = "A"; break;
    }

    // 组合文件名：big_red_A.png 或 small_black_K. png
    std::string path ="res/number/"+ sizePrefix + "_" + color + "_" + faceStr + ".png";

    CCLOG("CardView: Generated number image path: %s (face=%d, suit=%d, useBig=%d)",
        path.c_str(), cardFace, cardSuit, useBig);

    return path;
}

void CardView::setCardId(int cardId) {
    _cardId = cardId;
}

int CardView::getCardId() const {
    return _cardId;
}

void CardView::setOnClickCallback(CardClickCallback callback) {
    _onClickCallback = callback;
}

void CardView::setFaceUp(bool isFaceUp) {
    _isFaceUp = isFaceUp;

    // 显示或隐藏花色和点数
    if (_suitSprite) {
        _suitSprite->setVisible(isFaceUp);
    }
    if (_numberSprite) {
        _numberSprite->setVisible(isFaceUp);
    }

    CCLOG("CardView: Card id=%d setFaceUp=%d", _cardId, isFaceUp);
}

void CardView::playMoveAnimation(Vec2 targetPos, std::function<void()> callback) {
    // 创建移动动画
    auto moveTo = MoveTo::create(0.3f, targetPos);
    auto easeMove = EaseOut::create(moveTo, 2.0f);

    if (callback) {
        auto callbackAction = CallFunc::create(callback);
        auto sequence = Sequence::create(easeMove, callbackAction, nullptr);
        this->runAction(sequence);
    }
    else {
        this->runAction(easeMove);
    }

    CCLOG("CardView: Moving card id=%d to (%.1f, %.1f)", _cardId, targetPos.x, targetPos.y);
}

void CardView::playShakeAnimation() {
    // 左右抖动动画
    auto shake1 = MoveBy::create(0.05f, Vec2(-5, 0));
    auto shake2 = MoveBy::create(0.05f, Vec2(10, 0));
    auto shake3 = MoveBy::create(0.05f, Vec2(-10, 0));
    auto shake4 = MoveBy::create(0.05f, Vec2(10, 0));
    auto shake5 = MoveBy::create(0.05f, Vec2(-5, 0));

    auto sequence = Sequence::create(shake1, shake2, shake3, shake4, shake5, nullptr);
    this->runAction(sequence);

    CCLOG("CardView: Playing shake animation for card id=%d", _cardId);
}

void CardView::setTouchEnabled(bool enabled) {
    _touchEnabled = enabled;
}