#include "views/PlayfieldView.h"

const int PlayfieldView::PLAYFIELD_WIDTH;
const int PlayfieldView::PLAYFIELD_HEIGHT;

PlayfieldView::PlayfieldView() {
}

PlayfieldView::~PlayfieldView() {
    clear();
}

PlayfieldView* PlayfieldView::create() {
    PlayfieldView* ret = new (std::nothrow) PlayfieldView();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool PlayfieldView::init() {
    if (!Node::init()) {
        return false;
    }

    // 设置内容大小
    this->setContentSize(Size(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT));

    // 绘制边框（调试用）
    drawBorder();

    CCLOG("PlayfieldView initialized, size=(%d, %d)", PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);

    return true;
}

void PlayfieldView::drawBorder() {
    auto draw = DrawNode::create();
    Vec2 rect[5] = {
        Vec2(0, 0),
        Vec2(PLAYFIELD_WIDTH, 0),
        Vec2(PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT),
        Vec2(0, PLAYFIELD_HEIGHT),
        Vec2(0, 0)
    };
    draw->drawPoly(rect, 5, false, Color4F::WHITE);
    this->addChild(draw, -1);  // zOrder=-1，在卡牌下面
}

void PlayfieldView::addCard(CardView* card) {
    if (!card) {
        CCLOG("ERROR: PlayfieldView::addCard - card is null");
        return;
    }

    _cards.push_back(card);
    this->addChild(card);

    CCLOG("PlayfieldView:  Added card id=%d, total=%zu", card->getCardId(), _cards.size());
}

void PlayfieldView::removeCard(CardView* card) {
    if (!card) {
        return;
    }

    auto it = std::find(_cards.begin(), _cards.end(), card);
    if (it != _cards.end()) {
        _cards.erase(it);
        card->removeFromParent();
        CCLOG("PlayfieldView:  Removed card id=%d, remaining=%zu", card->getCardId(), _cards.size());
    }
}

CardView* PlayfieldView::getCardById(int cardId) {
    for (auto card : _cards) {
        if (card->getCardId() == cardId) {
            return card;
        }
    }
    return nullptr;
}

const std::vector<CardView*>& PlayfieldView::getCards() const {
    return _cards;
}

void PlayfieldView::clear() {
    for (auto card : _cards) {
        card->removeFromParent();
    }
    _cards.clear();
    CCLOG("PlayfieldView cleared");
}