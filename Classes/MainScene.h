#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(MainScene);
};