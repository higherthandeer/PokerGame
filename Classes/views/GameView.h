/**
 * @file GameView. h
 * @brief 游戏主视图
 *
 * 职责：
 * - 管理所有子视图
 * - 提供动画播放接口
 * - 处理UI交互
 */

#pragma once
#include "cocos2d.h"
#include "views/PlayfieldView.h"
#include "views/StackView.h"
#include "models/GameModel.h"
#include <functional>

USING_NS_CC;

// 前向声明
class GameController;

/**
 * @brief 游戏主视图类
 */
class GameView : public Scene {
public:
    using CardClickCallback = std::function<void(int)>;
    using UndoClickCallback = std::function<void()>;

    /**
     * @brief 创建游戏视图
     * @return 视图指针
     */
    static GameView* create();

    /**
     * @brief 初始化
     * @return 是否成功
     */
    virtual bool init() override;

    /**
     * @brief 从游戏模型创建所有卡牌视图
     * @param gameModel 游戏模型
     * @return 是否成功创建
     */
    bool createCardsFromModel(const GameModel& gameModel);

    /**
     * @brief 播放匹配动画
     * @param cardId 卡牌ID
     * @param targetPos 目标位置
     * @param callback 动画完成回调
     */
    void playMatchAnimation(int cardId, Vec2 targetPos, std::function<void()> callback = nullptr);

    /**
     * @brief 播放从备用牌堆抽牌的动画
     * @param cardId 卡牌ID
     * @param targetPos 目标位置
     * @param callback 动画完成回调
     */
    void playDrawFromReserveAnimation(int cardId, Vec2 targetPos, std::function<void()> callback = nullptr);

    /**
     * @brief 播放回退动画
     * @param cardId 卡牌ID
     * @param targetPos 目标位置
     * @param callback 动画完成回调
     */
    void playUndoAnimation(int cardId, Vec2 targetPos, std::function<void()> callback = nullptr);

    /**0
     * @brief 播放卡牌抖动动画（无法匹配时）
     * @param cardId 卡牌ID
     */
    void playCardShakeAnimation(int cardId);

    /**
     * @brief 显示胜利对话框
     */
    void showVictoryDialog();

    /**
     * @brief 设置回退按钮启用状态
     * @param enabled 是否启用
     */
    void setUndoButtonEnabled(bool enabled);

    /**
     * @brief 设置卡牌点击回调
     * @param callback 回调函数
     */
    void setOnCardClickCallback(CardClickCallback callback);

    /**
     * @brief 设置回退按钮点击回调
     * @param callback 回调函数
     */
    void setOnUndoClickCallback(UndoClickCallback callback);

    /**
     * @brief 获取桌面牌区视图
     * @return 视图指针
     */
    PlayfieldView* getPlayfieldView() { return _playfieldView; }

    /**
     * @brief 获取手牌区视图
     * @return 视图指针
     */
    StackView* getBaseStackView() { return _baseStackView; }

    /**
     * @brief 获取备用牌堆视图
     * @return 视图指针
     */
    StackView* getReserveStackView() { return _reserveStackView; }

private:
    GameView();
    virtual ~GameView();

    /**
     * @brief 初始化UI
     */
    void initUI();

    /**
     * @brief 根据ID查找卡牌视图
     * @param cardId 卡牌ID
     * @return 卡牌视图，未找到返回nullptr
     */
    CardView* findCardViewById(int cardId);

private:
    PlayfieldView* _playfieldView;      // 桌面牌区
    StackView* _baseStackView;          // 手牌区
    StackView* _reserveStackView;       // 备用牌堆

    MenuItemLabel* _undoMenuItem;      // 文字按钮菜单项
    LayerColor* _undoButtonBg;         // 按钮背景

    GameController* _controller;        // 游戏控制器

    CardClickCallback _onCardClickCallback;  // 卡牌点击回调
    UndoClickCallback _onUndoClickCallback;  // 回退点击回调
};