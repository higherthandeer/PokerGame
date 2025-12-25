/**
 * @file GameModelFromLevalGenerator.h
 * @brief 游戏模型生成服务
 *
 * 职责：
 * - 将静态配置（LevelConfig）转换为运行时数据（GameModel）
 * - 处理卡牌ID分配
 * - 处理初始手牌的抽取
 *
 * 注意：
 * - 无状态服务，不持有数据
 * - 提供静态方法
 */

#pragma once
#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"

 /**
  * @brief 游戏模型生成服务
  *
  * 提供从配置生成游戏模型的服务
  */
class GameModelFromLevelGenerator {
public:
    /**
     * @brief 从关卡配置生成游戏模型
     * @param config 关卡配置
     * @param outModel 输出的游戏模型（引用传递，修改原数据）
     * @return 是否成功生成
     */
    static bool generateFromConfig(const LevelConfig& config, GameModel& outModel);

private:
    GameModelFromLevelGenerator() = delete;  // 禁止实例化
};