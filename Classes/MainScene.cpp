#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "UserProfile.h"
#include "ServerCommunicator.h"
#include "RankScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

//로그인 하는 순간 로그인 버튼을 없애기 위함//
Menu* fbLoginBtnPtr = nullptr;
Menu* mainMenuBtnPtr = nullptr;
Label* titleLabelPtr = nullptr;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
/* Header for class org_cocos2dx_cpp_AppActivity */

#ifdef __cplusplus
extern "C" {
#endif
	/*
	* Class:     org_cocos2dx_cpp_AppActivity
	* Method:    loginSuccess
	* Signature: ()V
	* Description : JNI를 이용해서 페이스북 최초 로그인이 Success 되었을 시 해당 함수가 호출 되게 했습니다.
	                로그인이 성공하면, UserDefault를 이용해 해당 기기에 facebook의 고유 id와 닉네임을 저장합니다.
					그리고 로그인 버튼을 지우고 게임 메뉴를 띄웁니다.
	*/
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_loginSuccess(JNIEnv *env, jobject obj,jstring id, jstring nickname)
	{
		std::string s_id = JniHelper::jstring2string(id);
		std::string s_nickname = JniHelper::jstring2string(nickname);

		UserDefault::getInstance()->setStringForKey("user_id", s_id);
		UserDefault::getInstance()->setStringForKey("user_nickname", s_nickname);

		if (fbLoginBtnPtr != nullptr)
		{
			fbLoginBtnPtr->setVisible(false);
			mainMenuBtnPtr->setVisible(true);
		}
		if (titleLabelPtr != nullptr)
		{
			titleLabelPtr->setString(s_nickname);
			titleLabelPtr->setVisible(true);
		}
		ServerCommunicator::getInstance()->registerUser(s_id, s_nickname);

		//refresh scene
		//Director::getInstance()->replaceScene(MainScene::createScene());

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
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto sprite = Sprite::create("main/background.png");
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(sprite, 0);

	auto gameStartLabel = Label::createWithTTF("start", "fonts/font.ttf", 40.0f);
	
	//GAME START Label을 누르면 GameScene으로 Transition
	auto gameStartMenuitem = MenuItemLabel::create(gameStartLabel, [&](Ref* sender) {
		//transition to GameScene
		auto scene = TransitionPageTurn::create(1.0f, GameScene::createScene(), false);
		Director::getInstance()->replaceScene(scene);
	});

	auto rankingLabel = Label::createWithTTF("rank", "fonts/font.ttf", 40.0f);
	
	auto rankingMenuitem = MenuItemLabel::create(rankingLabel, [](Ref*) {
		auto scene = TransitionPageTurn::create(1.0f, RankScene::createScene(), false);
		Director::getInstance()->replaceScene(scene);
	});

	//facebook connect 버튼을 누르면 Jni로 facebookLogin native 메서드를 호출합니다. (안드로이드)
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

	//게임 스타트 버튼, 랭킹 버튼으로 메뉴 생성
	auto main_menu = Menu::create(gameStartMenuitem, rankingMenuitem,nullptr);

	addLoggedInUserInterface();         //로그인 된 사용자 인터페이스를 add

	main_menu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 200);
	
	main_menu->alignItemsVerticallyWithPadding(10);

	auto fbLoginMenu = Menu::createWithItem(fbLoginMenuitem);

	fbLoginMenu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 150);

	auto fbLoginLabel = Label::createWithTTF("click facebook button to login", "fonts/kor_font.ttf", 40.0f);

	fbLoginLabel->setPosition(30, -50);

	fbLoginMenuitem->addChild(fbLoginLabel);

	addChild(fbLoginMenu);
	addChild(main_menu);
	
	/*
	if (UserProfile::getInstance()->isLoggedIn()) //로그인 되어 있으면
	{
		main_menu->setVisible(true);
		fbLoginMenu->setVisible(false);
		nickNameLabel->setVisible(true);
	}
	else
	{
		main_menu->setVisible(false);
		fbLoginMenu->setVisible(true);
	}
	*/
	fbLoginMenu->setVisible(false);

	//

	mainMenuBtnPtr = main_menu;
	fbLoginBtnPtr = fbLoginMenu;

	this->setKeypadEnabled(true);

    return true;
}

void MainScene::addLoggedInUserInterface()
{
	std::string userNickName = UserProfile::getInstance()->getUserNickName();

	nickNameLabel = Label::createWithTTF(userNickName, "fonts/kor_font.ttf", 40.0f);
	auto subLabel = Label::createWithTTF("is connected!", "fonts/kor_font.ttf", 20.0f);

	Size visibleSize = Director::getInstance()->getVisibleSize();

	nickNameLabel->setPosition(visibleSize.width/2, 300);

	subLabel->setPosition(subLabel->getContentSize().width / 2, -subLabel->getContentSize().height / 2);

	nickNameLabel->addChild(subLabel);
	addChild(nickNameLabel);

	nickNameLabel->setVisible(false);
	titleLabelPtr = nickNameLabel;
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		ServerCommunicator::getInstance()->shutgameTimeUpdate();
		Director::getInstance()->end();
		SimpleAudioEngine::getInstance()->end();
	}
}
