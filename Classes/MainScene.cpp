#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
/* Header for class org_cocos2dx_cpp_AppActivity */

#ifdef __cplusplus
extern "C" {
#endif
	/*
	* Class:     org_cocos2dx_cpp_AppActivity
	* Method:    onSuccess
	* Signature: ()V
	*/
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_loginSuccess(JNIEnv *env, jobject obj,jstring id, jstring nickname)
	{
		std::string s_id = JniHelper::jstring2string(id);
		std::string s_nickname = JniHelper::jstring2string(nickname);

		UserDefault::getInstance()->setStringForKey("user_id", s_id);
		UserDefault::getInstance()->setStringForKey("user_nickname", s_nickname);
		
		log("updated profile.. user_id is %s, user_nickname is %s", s_id.c_str(), s_nickname.c_str());

		return;
	}

#ifdef __cplusplus
}
#endif

#endif

Scene* MainScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto sprite = Sprite::create("main/background.png");

	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);

	auto gameStartLabel = Label::createWithTTF("GAME START", "fonts/font.ttf", 40.0f);
	gameStartLabel->setColor(Color3B::BLACK);

	auto gameStartMenuitem = MenuItemLabel::create(gameStartLabel, [&](Ref* sender) {
		auto scene = TransitionPageTurn::create(1.0f, HelloWorld::createScene(), false);
		Director::getInstance()->replaceScene(scene);
	});

	auto rankingLabel = Label::createWithTTF("RANK", "fonts/font.ttf", 40.0f);
	rankingLabel->setColor(Color3B::BLACK);

	auto rankingMenuitem = MenuItemLabel::create(rankingLabel);

	auto fbLoginMenuitem = MenuItemImage::create("main/facebook_connect_button.png", "main/facebook_connect_button.png",
		[](Ref* sender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				JniMethodInfo t;

				if (JniHelper::getStaticMethodInfo(t
					, "org.cocos2dx.cpp.AppActivity"       //org.cocos2dx.cpp.AppActivity <-까지가 패키지 명이고 그 뒤가 자바 파일 이름
					, "facebookLogin"    //<- 함수이름
					, "()V"))
				{
					// 함수 호출
					t.env->CallStaticVoidMethod(t.classID, t.methodID);
					// Release
					t.env->DeleteLocalRef(t.classID);
				}
#endif

	});

	fbLoginMenuitem->setScale(0.7f);

	auto main_menu = Menu::create(gameStartMenuitem, rankingMenuitem, fbLoginMenuitem, nullptr);


	if (UserDefault::getInstance()->getStringForKey("user_id", "").compare("") != 0)
	{
		fbLoginMenuitem->setVisible(false);
		std::string userNickName = UserDefault::getInstance()->getStringForKey("user_nickname", "");

		auto nickNameLabel = Label::createWithTTF(userNickName, "fonts/kor_font.ttf", 40.0f);
		
		nickNameLabel->setColor(Color3B::BLACK);
		nickNameLabel->setPosition(100, 700);
		addChild(nickNameLabel);
	}

	main_menu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 200);

	main_menu->alignItemsVerticallyWithPadding(10);

	addChild(main_menu);

    return true;
}
