#pragma once

#include "cocos2d.h"

/*
	��ŷ�� �����ִ� Scene
*/

class RankScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(RankScene);
};