/**
 * @file AnimationUtils.h
 * @brief 动画辅助工具
 *
 * 职责：
 * - 提供常用的动画创建方法
 * - 统一动画参数（时间、缓动函数等）
 */

#pragma once
#include "cocos2d.h"
#include <functional>

USING_NS_CC;

/**
 * @brief 动画辅助工具类
 *
 * 提供创建常用动画的静态方法
 */
class AnimationUtils {
public:
    using AnimationCallback = std::function<void()>;

    /**
     * @brief 创建卡牌移动动画
     * @param node 要移动的节点
     * @param targetPos 目标位置
     * @param duration 动画时长（秒）
     * @param callback 动画完成后的回调
     */
    static void moveCardTo(Node* node, Vec2 targetPos, float duration, AnimationCallback callback = nullptr);

    /**
     * @brief 创建卡牌翻转动画
     * @param node 要翻转的节点
     * @param faceUp 是否翻到正面
     * @param duration 动画时长（秒）
     * @param callback 动画完成后的回调
     */
    static void flipCard(Node* node, bool faceUp, float duration, AnimationCallback callback = nullptr);

    /**
     * @brief 创建卡牌消失动画
     * @param node 要消失的节点
     * @param duration 动画时长（秒）
     * @param callback 动画完成后的回调
     */
    static void fadeOutCard(Node* node, float duration, AnimationCallback callback = nullptr);

private:
    AnimationUtils() = delete;  // 禁止实例化

    // 动画参数常量
    static const float kDefaultMoveDuration;   // 默认移动时长
    static const float kDefaultFlipDuration;   // 默认翻转时长
    static const float kDefaultFadeDuration;   // 默认淡出时长
};