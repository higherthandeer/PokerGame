#include "GameScene.h"
USING_NS_CC;

GameScene* GameScene::create() {
    GameScene* ret = new (std::nothrow) GameScene();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GameScene::init() {
    if (!Scene::init()) return false;
    auto gameView = GameView::create();
  
    if (!gameView) {
        CCLOG("GameView create failed!");
        return false;
    }else{
        CCLOG("GameView create successful!");
    }
    this->addChild(gameView);
    return true;
}