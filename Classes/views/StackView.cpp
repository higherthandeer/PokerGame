#include "views/StackView.h"

const int StackView::STACK_WIDTH;
const int StackView::STACK_HEIGHT;

StackView::StackView() {
}

StackView::~StackView() {
    clear();
}

StackView* StackView::create() {
    StackView* ret = new (std::nothrow) StackView();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool StackView::init() {
    if (!Node::init()) {
        return false;
    }

    this->setContentSize(Size(STACK_WIDTH, STACK_HEIGHT));

    CCLOG("StackView initialized");

    return true;
}

void StackView::addCard(CardView* card) {
    if (!card) {
        CCLOG("ERROR: StackView:: addCard - card is null");
        return;
    }

    _cards.push_back(card);
    this->addChild(card);

    // 重新布局
    layoutCards();

    CCLOG("StackView: Added card id=%d, total=%zu", card->getCardId(), _cards.size());
}

void StackView::removeCard(CardView* card) {
    if (!card) {
        return;
    }

    auto it = std::find(_cards.begin(), _cards.end(), card);
    if (it != _cards.end()) {
        _cards.erase(it);
        card->removeFromParent();

        // 重新布局
        layoutCards();

        CCLOG("StackView:  Removed card id=%d, remaining=%zu", card->getCardId(), _cards.size());
    }
}

CardView* StackView::getTopCard() {
    if (_cards.empty()) {
        return nullptr;
    }
    return _cards.back();
}

CardView* StackView::getCardById(int cardId) {
    for (auto card : _cards) {
        if (card->getCardId() == cardId) {
            return card;
        }
    }
    return nullptr;
}

const std::vector<CardView*>& StackView::getCards() const {
    return _cards;
}

void StackView::clear() {
    for (auto card : _cards) {
        card->removeFromParent();
    }
    _cards.clear();
    CCLOG("StackView cleared");
}

void StackView::layoutCards() {
    if (_cards.empty()) {
        return;
    }

    // 所有卡牌堆叠在同一位置
    Vec2 stackPos = Vec2(0, 0);

    for (size_t i = 0; i < _cards.size(); ++i) {
        _cards[i]->setPosition(stackPos);
        _cards[i]->setLocalZOrder(static_cast<int>(i));  // 后面的卡牌在上面
        _cards[i]->setVisible(true);  // 全部可见
    }

    CCLOG("StackView: Layout cards, count=%zu", _cards.size());
}