/**
 * @file UndoManager.h
 * @brief 回退管理器
 *
 * 职责：
 * - 管理回退记录栈
 * - 提供回退操作的接口
 * - 限制回退步数上限
 *
 * 注意：
 * - 作为 GameController 的成员，不实现为单例
 * - 不反向依赖 Controller
 * - 通过回调接口与其他模块交互
 */

#pragma once
#include "models/UndoModel.h"
#include <vector>
#include <functional>

 /**
  * @brief 回退管理器类
  *
  * 管理游戏操作的回退功能
  */
class UndoManager {
public:
    using UndoCallback = std::function<void(const UndoRecord&)>;

    UndoManager();
    ~UndoManager();

    /**
     * @brief 初始化管理器
     * @param maxSteps 最大回退步数
     */
    void init(int maxSteps = 100);

    /**
     * @brief 清空所有回退记录
     */
    void clear();

    /**
     * @brief 添加一条回退记录
     * @param record 回退记录
     */
    void pushRecord(const UndoRecord& record);

    /**
     * @brief 弹出最近的一条回退记录
     * @return 回退记录，如果栈为空则返回默认记录
     */
    UndoRecord popRecord();

    /**
     * @brief 检查是否可以回退
     * @return 是否有可回退的记录
     */
    bool canUndo() const;

    /**
     * @brief 获取当前回退步数
     * @return 回退记录数量
     */
    int getUndoCount() const;

private:
    std::vector<UndoRecord> _undoStack;  // 回退栈
    int _maxSteps;                       // 最大回退步数
};