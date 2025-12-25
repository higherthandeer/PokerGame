/**
 * @file GameModel. h
 * @brief 游戏数据模型
 *
 * 职责：
 * - 存储游戏的运行时状态数据
 * - 管理所有卡牌的数据
 * - 支持序列化和反序列化
 */

#pragma once
#include "models/CardModel.h"
#include <vector>

 /**
  * @brief 游戏数据模型
  *
  * 存储游戏当前状态的所有数据
  */
class GameModel {
public:
    GameModel();
    ~GameModel();

    // ===== 基础方法 =====
    void clear();
    int getNextCardId();

    // ===== 添加卡牌 =====
    void addCardToPlayfield(const CardModel& card);
    void addCardToBaseStack(const CardModel& card);
    void addCardToReserveStack(const CardModel& card);

    // ===== 移除卡牌 =====
    bool removeCardFromPlayfield(int cardId);
    bool removeCardFromBaseStack(int cardId);
    bool removeCardFromReserveStack(int cardId);

    // ===== 查询方法 =====
    CardModel getCardById(int cardId) const;
    CardModel getBaseStackTop() const;
    bool isCardInPlayfield(int cardId) const;
    bool isCardInBaseStack(int cardId) const;
    bool isCardInReserveStack(int cardId) const;

    // ===== 获取数据集合（只读访问）=====
    const std::vector<CardModel>& getPlayfield() const;
    const std::vector<CardModel>& getBaseStack() const;
    const std::vector<CardModel>& getReserveStack() const;

private:
    std::vector<CardModel> playfield;      // 桌面牌区
    std::vector<CardModel> baseStack;      // 手牌区
    std::vector<CardModel> reserveStack;   // 备用牌堆
    int _nextCardId;                       // ID计数器
};