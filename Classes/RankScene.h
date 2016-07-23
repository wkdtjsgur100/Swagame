#pragma once

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"
#include "ExtensionExport.h"
#include "cocos-ext.h"

/*
	랭킹을 보여주는 Scene
*/
class RankScene : public cocos2d::Layer, cocos2d::extension::TableViewDelegate, cocos2d::extension::TableViewDataSource
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);
	void parseJson(const char* json);
	void makeRankTable();

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

    CREATE_FUNC(RankScene);
};