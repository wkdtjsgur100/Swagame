#include "RankScene.h"
#include "ServerCommunicator.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"

#include "ExtensionExport.h"
#include "cocos-ext.h"

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
}
