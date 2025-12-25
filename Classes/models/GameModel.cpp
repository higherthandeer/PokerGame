#include "models/GameModel.h"
#include "cocos2d.h"

USING_NS_CC;

GameModel::GameModel() : _nextCardId(0) {
}

GameModel::~GameModel() {
}

void GameModel::clear() {
    playfield.clear();
    baseStack.clear();
    reserveStack.clear();
    _nextCardId = 0;
    CCLOG("GameModel cleared");
}

int GameModel::getNextCardId() {
    return _nextCardId++;
}

void GameModel::addCardToPlayfield(const CardModel& card) {
    playfield.push_back(card);
}

void GameModel::addCardToBaseStack(const CardModel& card) {
    baseStack.push_back(card);
}

void GameModel::addCardToReserveStack(const CardModel& card) {
    reserveStack.push_back(card);
}

bool GameModel::removeCardFromPlayfield(int cardId) {
    for (auto it = playfield.begin(); it != playfield.end(); ++it) {
        if (it->id == cardId) {
            playfield.erase(it);
            return true;
        }
    }
    return false;
}

bool GameModel::removeCardFromBaseStack(int cardId) {
    for (auto it = baseStack.begin(); it != baseStack.end(); ++it) {
        if (it->id == cardId) {
            baseStack.erase(it);
            return true;
        }
    }
    return false;
}

bool GameModel::removeCardFromReserveStack(int cardId) {
    for (auto it = reserveStack.begin(); it != reserveStack.end(); ++it) {
        if (it->id == cardId) {
            reserveStack.erase(it);
            return true;
        }
    }
    return false;
}

CardModel GameModel::getCardById(int cardId) const {
    // 在桌面牌区查找
    for (const auto& card : playfield) {
        if (card.id == cardId) return card;
    }

    // 在手牌区查找
    for (const auto& card : baseStack) {
        if (card.id == cardId) return card;
    }

    // 在备用牌堆查找
    for (const auto& card : reserveStack) {
        if (card.id == cardId) return card;
    }

    // 未找到
    CCLOG("WARNING: Card with id=%d not found", cardId);
    return CardModel();
}

CardModel GameModel::getBaseStackTop() const {
    if (baseStack.empty()) {
        return CardModel();
    }
    return baseStack.back();
}

bool GameModel::isCardInPlayfield(int cardId) const {
    for (const auto& card : playfield) {
        if (card.id == cardId) return true;
    }
    return false;
}

bool GameModel::isCardInBaseStack(int cardId) const {
    for (const auto& card : baseStack) {
        if (card.id == cardId) return true;
    }
    return false;
}

bool GameModel::isCardInReserveStack(int cardId) const {
    for (const auto& card : reserveStack) {
        if (card.id == cardId) return true;
    }
    return false;
}

const std::vector<CardModel>& GameModel::getPlayfield() const {
    return playfield;
}

const std::vector<CardModel>& GameModel::getBaseStack() const {
    return baseStack;
}

const std::vector<CardModel>& GameModel::getReserveStack() const {
    return reserveStack;
}