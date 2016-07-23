#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

/*
	���� �����ϱ� �� 
*/
class MainScene : public cocos2d::Layer
{
	cocos2d::Label* nickNameLabel;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	void addLoggedInUserInterface();

	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    CREATE_FUNC(MainScene);
};