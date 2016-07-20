#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

/*
	게임 시작하기 전 
*/
class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	void addLoggedInUserInterface();
    CREATE_FUNC(MainScene);
};