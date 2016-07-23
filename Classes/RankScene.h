#pragma once

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"

/*
	랭킹을 보여주는 Scene
*/
class RankScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);
	void parseJson(const char* json);
	void makeRankTable();

    CREATE_FUNC(RankScene);
};