#include "RankScene.h"
#include "ServerCommunicator.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "MainScene.h"

USING_NS_CC;
using namespace network;
using namespace rapidjson;
using namespace extension;

Scene* RankScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = RankScene::create();

	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool RankScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto sprite = Sprite::create("rank/background.png");
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(sprite, 0);

	ServerCommunicator::getInstance()->requestRankDatas(CC_CALLBACK_2(RankScene::onHttpRequestCompleted, this));
	
	this->setKeypadEnabled(true);

	auto backLabel = Label::createWithTTF("<- Back", "fonts/font.ttf", 50.0f);

	auto backBtn = MenuItemLabel::create(backLabel, [&](Ref*) {
		auto scene = TransitionPageTurn::create(1.0f, MainScene::createScene(), true);
		Director::getInstance()->replaceScene(scene);
	});
	
	auto backMenu = Menu::createWithItem(backBtn);

	backMenu->setPosition(visibleSize.width / 2, 100);
	addChild(backMenu);

    return true;
}

void RankScene::onHttpRequestCompleted(HttpClient * sender, HttpResponse * response)
{
	if (!response)
		return;

	if (!response->isSucceed())
	{
		cocos2d::log("response failed");
		cocos2d::log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	std::string s_response;

	for (unsigned int i = 0; i < buffer->size(); i++)
		s_response.push_back((*buffer)[i]);

	const char* json = s_response.c_str();
	
	parseJson(json);

	loadProfilePictures();
	
	makeRankTable();
}

void RankScene::parseJson(const char* json)
{
	Document document; //json Ã³¸®¿ë °´Ã¼.

	log("received data : %s",json);

	std::string parsed_json = "{\"user_list\":";
	parsed_json += json;
	parsed_json.push_back('}');


	if (document.Parse<0>(parsed_json.c_str()).HasParseError()) //parse Error°¡ ¶ß¸é
	{
		log("Parse Error");
		return;
	}

	if (!document.IsObject())
	{
		log("parsed_json is not a object");
		return;
	}

	if (document.HasMember("user_list"))
	{
		for (rapidjson::SizeType i = 0; i<document["user_list"].Size(); i++)
		{
			if (document["user_list"][i].HasMember("fields"))
			{
				RankCellData rcd;

				if (document["user_list"][i]["fields"].HasMember("user_nickname"))
				{
					if (document["user_list"][i]["fields"]["user_nickname"].IsString())
						rcd.nick_name = document["user_list"][i]["fields"]["user_nickname"].GetString();
				}
				if (document["user_list"][i]["fields"].HasMember("user_id"))
				{
					if (document["user_list"][i]["fields"]["user_id"].IsString())
						rcd.id = document["user_list"][i]["fields"]["user_id"].GetString();
				}
				if (document["user_list"][i]["fields"].HasMember("user_score"))
				{
					if (document["user_list"][i]["fields"]["user_score"].IsInt())
						rcd.score = StringUtils::format("%d", document["user_list"][i]["fields"]["user_score"].GetInt());
				}
				rankDatas.push_back(rcd);

				log("rcd is got a %s,%s,%s", rcd.nick_name.c_str(), rcd.id.c_str(), rcd.score.c_str());
			}
		}
		document["user_list"].Clear();
	}
}

void RankScene::makeRankTable()
{
	Size winSize = Director::getInstance()->getVisibleSize();

	auto tableView = TableView::create(this, Size(480, 600));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Vec2(0, 150));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	
	this->addChild(tableView);
	tableView->reloadData();
}

void RankScene::loadProfilePictures()
{
	//extension method
}

void RankScene::onExit()
{
	Layer::onExit();

	rankDatas.clear();
}


Size RankScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(480, 70);
}

TableViewCell* RankScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto string = StringUtils::format("%ld", static_cast<long>(idx+1));
	RankCellData cellData;

	try {
		cellData = rankDatas.at(idx);
	}
	catch (const std::out_of_range& oor) {
	}

	TableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new (std::nothrow) RankTableViewCell();
		cell->autorelease();

		
		auto label = Label::createWithTTF(string, "fonts/font.ttf", 50.0f);
		label->setAnchorPoint(Vec2::ZERO);
		label->setTag(100);

		Sprite* profileSpr;

		if (cellData.profileTex != nullptr)
			profileSpr = Sprite::createWithTexture(cellData.profileTex);
		else
			profileSpr = Sprite::create();

		profileSpr->setTag(103);

		auto nickNameLabel = Label::createWithTTF(cellData.nick_name, "fonts/font.ttf", 50.0f);
		nickNameLabel->setAnchorPoint(Vec2::ZERO);
		nickNameLabel->setTag(101);
		nickNameLabel->setPosition(100, 0);
		
		auto scoreLabel = Label::createWithTTF(cellData.score, "fonts/font.ttf", 50.0f);
		scoreLabel->setAnchorPoint(Vec2::ZERO);
		scoreLabel->setTag(102);
		scoreLabel->setPosition(350, 0);

		cell->addChild(label);
		cell->addChild(nickNameLabel);
		cell->addChild(scoreLabel);
		cell->addChild(profileSpr);
	}
	else
	{
		auto label = (Label*)cell->getChildByTag(100);
		label->setString(string);

		auto nickNameLabel = (Label*)cell->getChildByTag(101);
		nickNameLabel->setString(cellData.nick_name);	

		auto scoreLabel = (Label*)cell->getChildByTag(102);
		scoreLabel->setString(cellData.score);

		auto profileSpr = (Sprite*)cell->getChildByTag(103);
		if(cellData.profileTex != nullptr)
			profileSpr->setTexture(cellData.profileTex);
	}


	return cell;
}

ssize_t RankScene::numberOfCellsInTableView(TableView *table)
{
	return 20;
}

void RankScene::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event * event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)
	{
		auto scene = TransitionPageTurn::create(1.0f, MainScene::createScene(), true);
		Director::getInstance()->replaceScene(scene);
	}
}

void RankScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", static_cast<long>(cell->getIdx()));
}

bool RankTableViewCell::init()
{
	if (!TableViewCell::init())
		return false;

	return true;
}

RankCellData::RankCellData()
{
	id = "0";
	nick_name = "empty";
	score = "0";
	profileTex = nullptr;
}

RankCellData::~RankCellData()
{
	if (profileTex != nullptr)
		profileTex->release();
}
