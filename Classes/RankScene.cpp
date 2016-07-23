#include "RankScene.h"
#include "ServerCommunicator.h"

USING_NS_CC;

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

    return true;
}