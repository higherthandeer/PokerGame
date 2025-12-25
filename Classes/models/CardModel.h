/**
 * @file CardModel.h
 * @brief 卡牌数据模型
 *
 * 职责：
 * - 存储单张卡牌的运行时数据
 * - 支持序列化和反序列化（用于存档）
 */

#pragma once

 /**
  * @brief 花色类型枚举
  */
enum CardSuitType {
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

/**
 * @brief 点数类型枚举
 */
enum CardFaceType {
    CFT_NONE = -1,
    CFT_ACE,        // A
    CFT_TWO,        // 2
    CFT_THREE,      // 3
    CFT_FOUR,       // 4
    CFT_FIVE,       // 5
    CFT_SIX,        // 6
    CFT_SEVEN,      // 7
    CFT_EIGHT,      // 8
    CFT_NINE,       // 9
    CFT_TEN,        // 10
    CFT_JACK,       // J
    CFT_QUEEN,      // Q
    CFT_KING,       // K
    CFT_NUM_CARD_FACE_TYPES
};

/**
 * @brief 卡牌数据模型
 *
 * 存储单张卡牌的所有运行时数据
 */
struct CardModel {
    int id;           // 卡牌唯一ID
    int face;         // 卡牌点数 (0-12)
    int suit;         // 卡牌花色 (0-3)
    bool isFaceUp;    // 是否正面朝上
    bool isRemoved;   // 是否已被移除
    float posX;       // X坐标
    float posY;       // Y坐标

    CardModel()
        : id(-1), face(0), suit(0), isFaceUp(false),
        isRemoved(false), posX(0), posY(0) {
    }

    CardModel(int i, int f, int s, bool faceUp, bool removed, float x, float y)
        : id(i), face(f), suit(s), isFaceUp(faceUp),
        isRemoved(removed), posX(x), posY(y) {
    }
};