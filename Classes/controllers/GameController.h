/**
 * @file GameController.h
 * @brief 游戏主控制器
 *
 * 职责：
 * - 管理整个游戏流程
 * - 协调 Model 和 View
 * - 处理用户输入和游戏逻辑
 * - 管理 UndoManager
 */

#pragma once
#include "cocos2d.h"
#include "models/GameModel.h"
#include "managers/UndoManager.h"
#include <functional>

USING_NS_CC;

// 前向声明
class GameView;

/**
 * @brief 游戏主控制器类
 *
 * 管理游戏的核心逻辑和流程
 */
class GameController {
public:
    GameController();
    ~GameController();

    /**
     * @brief 初始化控制器
     * @param gameView 游戏视图指针
     * @return 是否成功初始化
     */
    bool init(GameView* gameView);

    /**
     * @brief 开始游戏
     * @param levelId 关卡ID（暂时未使用，直接加载 level1. json）
     * @return 是否成功开始
     */
    bool startGame(int levelId = 1);

    /**
     * @brief 处理卡牌点击事件
     * @param cardId 被点击的卡牌ID
     */
    void onCardClicked(int cardId);

    /**
     * @brief 处理回退按钮点击
     */
    void onUndoClicked();

    /**
     * @brief 检查游戏是否胜利
     * @return 是否胜利
     */
    bool checkVictory() const;

private:
    /**
     * @brief 检查两张卡牌是否可以匹配
     * @param card1 卡牌1
     * @param card2 卡牌2
     * @return 是否可以匹配
     */
    bool canMatch(const CardModel& card1, const CardModel& card2) const;

    /**
     * @brief 处理桌面牌点击
     * @param cardId 卡牌ID
     */
    void handlePlayfieldCardClick(int cardId);

    /**
     * @brief 处理备用牌堆点击
     * @param cardId 卡牌ID
     */
    void handleReserveStackClick(int cardId);

    /**
     * @brief 执行回退操作
     * @param record 回退记录
     */
    void executeUndo(const UndoRecord& record);

    /**
     * @brief 更新回退按钮状态
     */
    void updateUndoButton();

private:
    GameModel* _gameModel;          // 游戏数据模型
    GameView* _gameView;            // 游戏视图
    UndoManager* _undoManager;      // 回退管理器
};