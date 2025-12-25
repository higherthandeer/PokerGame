/**
 * @file PlayfieldView.h
 * @brief 桌面牌区视图
 *
 * 职责：
 * - 显示桌面牌区的所有卡牌
 * - 管理桌面牌区的布局
 */

#pragma once
#include "cocos2d.h"
#include "views/CardView.h"
#include <vector>

USING_NS_CC;

/**
 * @brief 桌面牌区视图类
 */
class PlayfieldView : public Node {
public:
    /**
     * @brief 创建桌面牌区视图
     * @return 视图指针
     */
    static PlayfieldView* create();

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
     * @brief 根据ID获取卡牌
     * @param cardId 卡牌ID
     * @return 卡牌视图指针，未找到返回nullptr
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

private:
    PlayfieldView();
    virtual ~PlayfieldView();

    /**
     * @brief 绘制边框
     */
    void drawBorder();

private:
    std::vector<CardView*> _cards;  // 所有卡牌

    static const int PLAYFIELD_WIDTH = 1080;
    static const int PLAYFIELD_HEIGHT = 1500;
};