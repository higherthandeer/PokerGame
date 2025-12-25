# PokerGame

基于 Cocos2d-x 的扑克牌消除游戏。

## 开发动机

redinfinity.net红海无线测试Demo

## 游戏简介

卡牌匹配消除游戏，玩家需要点击桌面上的卡牌，与手牌进行匹配消除。

## 技术栈

- **引擎**: Cocos2d-x 4.0
- **语言**: C++17
- **构建工具**: CMake 3.6+
- **平台**: Windows

## 项目结构

```
PokerGame/
├── Classes/           # 游戏源代码
│   ├── configs/       # 配置加载器
│   ├── controllers/   # 控制器层
│   ├── managers/      # 管理器
│   ├── models/        # 数据模型
│   ├── services/      # 业务服务
│   ├── utils/         # 工具类
│   └── views/         # 视图层
├── Resources
    ├─fonts
    └─res
        ├─levels	   # 关卡配置
        ├─number	   # 卡牌数字
        └─suits        # 卡牌花色
├── CMakeLists.txt     # CMake 构建配置
└── README.md          # 项目说明
```

## 前置要求

### Windows

- Visual Studio 2019 或更高版本
- CMake 3.6+
- Git

## 编译和运行

#### Windows

```bash
# 项目根目录下运行
cmake -S . -B build_win32 -A Win32
```

打开 `build_win32/PokerGame.slnx`

按 `F5` 运行调试

## 游戏玩法

1. 点击桌面上的卡牌
2. 如果卡牌与手牌的点数相差为 1 或 12，则匹配成功
2. 点击回退可撤销一步操作
3. 匹配成功的卡牌移动到手牌区
4. 清除桌面所有卡牌即为胜利
