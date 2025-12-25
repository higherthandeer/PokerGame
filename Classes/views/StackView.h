/**
 * @file StackView.h
 * @brief 牌堆视图（手牌区和备用牌堆通用）
 *
 * 职责：
 * - 显示牌堆中的卡牌
 * - 管理牌堆布局
 */

#pragma once
#include "cocos2d.h"
#include "views/CardView.h"
#include <vector>

USING_NS_CC;

/**
 * @brief 牌堆视图类
 *
 * 用于手牌区和备用牌堆
 */
class StackView : public Node {
public:
    /**
     * @brief 创建牌堆视图
     * @return 视图指针
     */
    static StackView* create();

    /**
     * @brief 初始化
     * @return 是否成功
     */
    virtual bool init() override;

    /**
     * @brief 添加卡牌
     * @param card 卡牌视图
     */
    void addCard(CardView* card);

    /**
     * @brief 移除卡牌
     * @param card 卡牌视图
     */
    void removeCard(CardView* card);

    /**
     * @brief 获取顶牌
     * @return 顶牌视图，为空返回nullptr
     */
    CardView* getTopCard();

    /**
     * @brief 根据ID获取卡牌
     * @param cardId 卡牌ID
     * @return 卡牌视图，未找到返回nullptr
     */
    CardView* getCardById(int cardId);

    /**
     * @brief 获取所有卡牌
     * @return 卡牌列表
     */
    const std::vector<CardView*>& getCards() const;

    /**
     * @brief 清空所有卡牌
     */
    void clear();

    /**
     * @brief 重新布局卡牌
     */
    void layoutCards();

private:
    StackView();
    virtual ~StackView();

private:
    std::vector<CardView*> _cards;  // 所有卡牌

    static const int STACK_WIDTH = 200;
    static const int STACK_HEIGHT = 300;
};