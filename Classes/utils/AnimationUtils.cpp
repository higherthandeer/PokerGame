#include "utils/AnimationUtils.h"

// 动画参数常量定义
const float AnimationUtils:: kDefaultMoveDuration = 0.3f;
const float AnimationUtils::kDefaultFlipDuration = 0.3f;
const float AnimationUtils::kDefaultFadeDuration = 0.3f;

void AnimationUtils::moveCardTo(Node* node, Vec2 targetPos, float duration, AnimationCallback callback) {
    if (!node) {
        CCLOG("AnimationHelper:: moveCardTo - node is null");
        return;
    }
    
    // 创建移动动作
    auto moveTo = MoveTo::create(duration, targetPos);
    auto easeMove = EaseOut::create(moveTo, 2.0f);  // 缓出效果
    
    // 如果有回调，添加回调动作
    if (callback) {
        auto callbackAction = CallFunc::create(callback);
        auto sequence = Sequence::create(easeMove, callbackAction, nullptr);
        node->runAction(sequence);
    } else {
        node->runAction(easeMove);
    }
    
    CCLOG("AnimationHelper:  Moving card to (%.1f, %.1f) in %.2f seconds", 
          targetPos.x, targetPos.y, duration);
}

void AnimationUtils::flipCard(Node* node, bool faceUp, float duration, AnimationCallback callback) {
    if (!node) {
        CCLOG("AnimationHelper::flipCard - node is null");
        return;
    }
    
    // 翻转动画：先缩小到0，再放大
    auto scaleX1 = ScaleTo::create(duration / 2, 0.0f, 1.0f);
    auto scaleX2 = ScaleTo::create(duration / 2, 1.0f, 1.0f);
    
    // 在中间时刻切换正反面
    auto flipFunc = CallFunc::create([node, faceUp]() {
        // 这里需要根据你的 CardView 实现来切换正反面
        // 例如：((CardView*)node)->setFaceUp(faceUp);
    });
    
    Sequence* sequence = nullptr;
    if (callback) {
        auto callbackAction = CallFunc::create(callback);
        sequence = Sequence::create(scaleX1, flipFunc, scaleX2, callbackAction, nullptr);
    } else {
        sequence = Sequence::create(scaleX1, flipFunc, scaleX2, nullptr);
    }
    
    node->runAction(sequence);
    
    CCLOG("AnimationUtils: Flipping card to %s in %.2f seconds", 
          faceUp ? "face up" : "face down", duration);
}

void AnimationUtils::fadeOutCard(Node* node, float duration, AnimationCallback callback) {
    if (!node) {
        CCLOG("AnimationHelper::fadeOutCard - node is null");
        return;
    }
    
    auto fadeOut = FadeOut::create(duration);
    
    if (callback) {
        auto callbackAction = CallFunc::create(callback);
        auto sequence = Sequence::create(fadeOut, callbackAction, nullptr);
        node->runAction(sequence);
    } else {
        node->runAction(fadeOut);
    }
    
    CCLOG("AnimationUtils: Fading out card in %.2f seconds", duration);
}
