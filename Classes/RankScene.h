#pragma once

#include "cocos2d.h"

#include "ExtensionExport.h"
#include "cocos-ext.h"

#include "network/HttpClient.h"
#include "network/HttpResponse.h"


class RankTableViewCell : public cocos2d::extension::TableViewCell
{
public:
	bool init() override;
};

/*
	랭킹 셀 하나에 구성 된 데이터들
*/
class RankCellData
{
public:
	std::string id;
	std::string nick_name;
	std::string score;
	cocos2d::Texture2D* profileTex;

	RankCellData();
	~RankCellData();
};

/*
	랭킹을 보여주는 Scene
*/

class RankScene : public cocos2d::Layer, cocos2d::extension::TableViewDelegate, cocos2d::extension::TableViewDataSource
{
private:
	std::vector<RankCellData> rankDatas;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);
	void parseJson(const char* json);
	void makeRankTable();
	void loadProfilePictures();

	virtual void onExit() override;

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);

    CREATE_FUNC(RankScene);
};