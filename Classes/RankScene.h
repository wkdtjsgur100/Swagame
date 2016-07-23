#pragma once

#include "cocos2d.h"

/*
	랭킹을 보여주는 Scene
*/

class RankScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(RankScene);
};