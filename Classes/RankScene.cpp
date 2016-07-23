#include "RankScene.h"
#include "ServerCommunicator.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"

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

	auto sprite = Sprite::create("main/background.png");
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(sprite, 0);

	ServerCommunicator::getInstance()->requestRankDatas(CC_CALLBACK_2(RankScene::onHttpRequestCompleted, this));

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
				if (document["user_list"][i]["fields"].HasMember("user_nickname"))
					log(document["user_list"][i]["fields"]["user_nickname"].GetString());
			}
		}
		document["user_list"].Clear();
	}
}

void RankScene::makeRankTable()
{
	Size winSize = Director::getInstance()->getVisibleSize();

	auto tableView = TableView::create(this, Size(60, 250));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Vec2(winSize.width - 150, winSize.height / 2 - 120));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	
	this->addChild(tableView);
	tableView->reloadData();
}


Size RankScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	if (idx == 2) {
		return Size(100, 100);
	}
	return Size(60, 60);
}

TableViewCell* RankScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto string = StringUtils::format("%ld", static_cast<long>(idx));
	TableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new (std::nothrow) CustomTableViewCell();
		cell->autorelease();
		auto sprite = Sprite::create("Images/Icon.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(0, 0));
		cell->addChild(sprite);

		auto label = Label::createWithSystemFont(string, "Helvetica", 20.0);
		label->setPosition(Vec2::ZERO);
		label->setAnchorPoint(Vec2::ZERO);
		label->setTag(123);
		cell->addChild(label);
	}
	else
	{
		auto label = (Label*)cell->getChildByTag(123);
		label->setString(string);
	}


	return cell;
}

ssize_t RankScene::numberOfCellsInTableView(TableView *table)
{
	return 20;
}

void RankScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", static_cast<long>(cell->getIdx()));
}