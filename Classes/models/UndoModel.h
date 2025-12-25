/**
 * @file UndoModel.h
 * @brief 回退数据模型
 *
 * 职责：
 * - 存储单次操作的回退信息
 * - 支持序列化和反序列化
 */

#pragma once
#include "cocos2d.h"

USING_NS_CC;

/**
 * @brief 操作类型枚举
 */
enum class UndoActionType {
    NONE = 0,
    MOVE_CARD,              // 移动卡牌（桌面牌到手牌区）
    DRAW_FROM_RESERVE,      // 从备用牌堆抽牌到手牌区
};

/**
 * @brief 卡牌位置区域枚举
 */
enum class CardArea {
    PLAYFIELD = 0,    // 桌面牌区
    BASE_STACK = 1,   // 手牌区
    RESERVE_STACK = 2 // 备用牌堆
};

/**
 * @brief 回退记录结构
 *
 * 存储一次操作的完整信息，用于回退
 */
struct UndoRecord {
    UndoActionType actionType;  // 操作类型
    int cardId;                 // 卡牌ID
    CardArea fromArea;          // 源区域
    CardArea toArea;            // 目标区域
    Vec2 fromPos;               // 源位置
    Vec2 toPos;                 // 目标位置

    UndoRecord()
        : actionType(UndoActionType::NONE)
        , cardId(-1)
        , fromArea(CardArea::PLAYFIELD)
        , toArea(CardArea::BASE_STACK)
        , fromPos(Vec2::ZERO)
        , toPos(Vec2::ZERO) {
    }

    UndoRecord(UndoActionType type, int id, CardArea from, CardArea to, Vec2 fromP, Vec2 toP)
        : actionType(type)
        , cardId(id)
        , fromArea(from)
        , toArea(to)
        , fromPos(fromP)
        , toPos(toP) {
    }
};