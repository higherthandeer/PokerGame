/**
 * @file LevelConfig.h
 * @brief 关卡配置数据结构
 *
 * 职责：
 * - 定义关卡的静态配置数据结构
 * - 包含桌面牌区和备用牌堆的初始卡牌配置
 */

#pragma once
#include "cocos2d.h"
#include <vector>

USING_NS_CC;

/**
 * @brief 卡牌配置结构
 */
struct CardConfig {
    int face;        // 卡牌点数 (0-12:  A-K)
    int suit;        // 卡牌花色 (0-3: 梅花、方块、红桃、黑桃)
    Vec2 position;   // 卡牌位置

    CardConfig() : face(0), suit(0), position(Vec2::ZERO) {}
    CardConfig(int f, int s, Vec2 pos) : face(f), suit(s), position(pos) {}
};

/**
 * @brief 关卡配置类
 *
 * 存储从JSON文件加载的关卡静态配置数据
 */
class LevelConfig {
public:
    std::vector<CardConfig> playfieldCards;  // 桌面牌区的卡牌配置
    std::vector<CardConfig> stackCards;      // 备用牌堆的卡牌配置

    /**
     * @brief 清空配置数据
     */
    void clear() {
        playfieldCards.clear();
        stackCards.clear();
    }
};