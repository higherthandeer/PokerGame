/**
 * @file CardView.h
 * @brief 卡牌视图
 *
 * 职责：
 * - 显示单张卡牌（由底图+花色+点数组成）
 * - 处理卡牌的触摸事件
 * - 播放卡牌相关动画
 */

#pragma once
#include "cocos2d.h"
#include <functional>

USING_NS_CC;

/**
 * @brief 卡牌视图类
 */
class CardView : public Node {
public:
    using CardClickCallback = std::function<void(int)>;

    /**
     * @brief 创建卡牌视图
     * @param cardFace 卡牌点数 (0-12: A-K)
     * @param cardSuit 卡牌花色 (0-3: 梅花、方块、红桃、黑桃)
     * @param isFaceUp 是否正面朝上
     * @param useBigCard 是否使用大卡牌图片（默认true）
     * @return 卡牌视图指针
     */
    static CardView* create(int cardFace, int cardSuit, bool isFaceUp = true, bool useBigCard = true);

    /**
     * @brief 初始化卡牌
     */
    virtual bool init(int cardFace, int cardSuit, bool isFaceUp, bool useBigCard);

    /**
     * @brief 设置卡牌ID
     */
    void setCardId(int cardId);

    /**
     * @brief 获取卡牌ID
     */
    int getCardId() const;

    /**
     * @brief 获取卡牌点数
     */
    int getFace() const { return _cardFace; }

    /**
     * @brief 获取卡牌花色
     */
    int getSuit() const { return _cardSuit; }

    /**
     * @brief 设置点击回调
     */
    void setOnClickCallback(CardClickCallback callback);

    /**
     * @brief 设置是否正面朝上
     */
    void setFaceUp(bool isFaceUp);

    /**
     * @brief 播放移动动画
     */
    void playMoveAnimation(Vec2 targetPos, std::function<void()> callback = nullptr);

    /**
     * @brief 播放抖动动画（无法匹配时）
     */
    void playShakeAnimation();

    /**
     * @brief 设置是否可点击
     */
    void setTouchEnabled(bool enabled);

private:
    CardView();
    virtual ~CardView();

    /**
     * @brief 设置触摸监听器
     */
    void setupTouchListener();

    /**
     * @brief 卡牌被点击时的回调
     */
    void onCardClicked();

    /**
     * @brief 获取点数图片路径
     */
    std::string getNumberImagePath(int cardFace, int cardSuit, bool useBig) const;

    /**
     * @brief 获取花色图片路径
     */
    std::string getSuitImagePath(int cardSuit) const;

private:
    int _cardId;                    // 卡牌ID
    int _cardFace;                  // 卡牌点数 (0-12)
    int _cardSuit;                  // 卡牌花色 (0-3)
    bool _isFaceUp;                 // 是否正面朝上
    bool _useBigCard;               // 是否使用大卡牌

    Sprite* _bgSprite;              // 底图精灵
    Sprite* _suitSprite;            // 花色精灵
    Sprite* _numberSprite;          // 点数精灵

    CardClickCallback _onClickCallback;  // 点击回调
    bool _touchEnabled;             // 是否可点击
};