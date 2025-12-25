/**
 * @file LevelConfigLoader. h
 * @brief 关卡配置加载器
 *
 * 职责：
 * - 从JSON文件加载关卡配置
 * - 解析配置数据并转换为LevelConfig对象
 */

#pragma once
#include "configs/models/LevelConfig.h"
#include <string>

 /**
  * @brief 关卡配置加载器
  *
  * 提供静态方法从JSON文件加载关卡配置
  */
class LevelConfigLoader {
public:
    /**
     * @brief 从JSON文件加载关卡配置
     * @param filename JSON文件名（相对于Resources目录）
     * @return 加载的关卡配置对象
     */
    static LevelConfig loadFromFile(const std::string& filename);

private:
    LevelConfigLoader() = delete;  // 禁止实例化
};