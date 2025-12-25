#include "services/GameModelFromLevelGenerator.h"
#include "cocos2d.h"

USING_NS_CC;

bool GameModelFromLevelGenerator::generateFromConfig(const LevelConfig& config, GameModel& outModel) {
    CCLOG("========== GameModelGenerator::generateFromConfig ==========");

    // 清空模型
    outModel.clear();

    int cardId = 0;

    // 生成备用牌堆的卡牌
    CCLOG("Generating reserve stack cards...");
    for (const auto& cardCfg : config.stackCards) {
        CardModel card;
        card.id = cardId++;
        card.face = cardCfg.face;
        card.suit = cardCfg.suit;
        card.isFaceUp = true;
        card.isRemoved = false;
        card.posX = cardCfg.position.x;
        card.posY = cardCfg.position.y;

        outModel.addCardToReserveStack(card);

        CCLOG("  Card id=%d, face=%d, suit=%d", card.id, card.face, card.suit);
    }

    // 从备用牌堆抽一张到手牌区
    if (!outModel.getReserveStack().empty()) {
        CardModel initialCard = outModel.getReserveStack().back();
        outModel.removeCardFromReserveStack(initialCard.id);
        outModel.addCardToBaseStack(initialCard);

        CCLOG("Moved initial card to base stack:  id=%d", initialCard.id);
    }
    else {
        CCLOG("WARNING: No cards in reserve stack to draw initial card");
    }

    // 生成桌面牌区的卡牌
    CCLOG("Generating playfield cards...");
    for (const auto& cardCfg : config.playfieldCards) {
        CardModel card;
        card.id = cardId++;
        card.face = cardCfg.face;
        card.suit = cardCfg.suit;
        card.isFaceUp = true;
        card.isRemoved = false;
        card.posX = cardCfg.position.x;
        card.posY = cardCfg.position.y;

        outModel.addCardToPlayfield(card);

        CCLOG("  Card id=%d, face=%d, suit=%d", card.id, card.face, card.suit);
    }

    // 验证数据
    CCLOG("===== Generated GameModel =====");
    CCLOG("Playfield:  %zu cards", outModel.getPlayfield().size());
    CCLOG("BaseStack: %zu cards", outModel.getBaseStack().size());
    CCLOG("ReserveStack: %zu cards", outModel.getReserveStack().size());
    CCLOG("================================");

    return true;
}