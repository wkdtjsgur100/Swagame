#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "UserProfile.h"

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
	* Method:    loginSuccess
	* Signature: ()V
	* Description : JNI�� �̿��ؼ� ���̽��� ���� �α����� Success �Ǿ��� �� �ش� �Լ��� ȣ�� �ǰ� �߽��ϴ�.
	                �α����� �����ϸ�, UserDefault�� �̿��� �ش� ��⿡ facebook�� ���� id�� �г����� �����մϴ�.
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
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto sprite = Sprite::create("main/background.png");
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(sprite, 0);

	auto gameStartLabel = Label::createWithTTF("GAME START", "fonts/font.ttf", 40.0f);
	gameStartLabel->setColor(Color3B::BLACK);

	//GAME START Label�� ������ GameScene���� Transition
	auto gameStartMenuitem = MenuItemLabel::create(gameStartLabel, [&](Ref* sender) {
		//transition to GameScene
		auto scene = TransitionPageTurn::create(1.0f, GameScene::createScene(), false);
		Director::getInstance()->replaceScene(scene);
	});

	auto rankingLabel = Label::createWithTTF("RANK", "fonts/font.ttf", 40.0f);
	rankingLabel->setColor(Color3B::BLACK);

	auto rankingMenuitem = MenuItemLabel::create(rankingLabel);

	//facebook connect ��ư�� ������ Jni�� facebookLogin native �޼��带 ȣ���մϴ�. (�ȵ���̵�)
	auto fbLoginMenuitem = MenuItemImage::create("main/facebook_connect_button.png", "main/facebook_connect_button.png",
		[](Ref* sender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				JniMethodInfo t;

				if (JniHelper::getStaticMethodInfo(t
					, "org.cocos2dx.cpp.AppActivity"       //org.cocos2dx.cpp.AppActivity <-������ ��Ű�� ���̰� �� �ڰ� �ڹ� ���� �̸�
					, "facebookLogin"    //<- �Լ��̸�
					, "()V"))
				{
					// �Լ� ȣ��
					t.env->CallStaticVoidMethod(t.classID, t.methodID);
					// Release
					t.env->DeleteLocalRef(t.classID);
				}
#endif

	});

	fbLoginMenuitem->setScale(0.7f);

	//���� ��ŸƮ ��ư, ��ŷ ��ư, ��� �α��� ��ư���� �޴� ����
	auto main_menu = Menu::create(gameStartMenuitem, rankingMenuitem, fbLoginMenuitem, nullptr);

	if (UserProfile::getInstance()->isLoggedIn())
	{
		fbLoginMenuitem->setVisible(false);	//��� �α��� ��ư�� ���ְ�
		addLoggedInUserInterface();         //�α��� �� ����� �������̽��� add
	}

	main_menu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 200);
	main_menu->alignItemsVerticallyWithPadding(10);

	addChild(main_menu);

    return true;
}

void MainScene::addLoggedInUserInterface()
{
	std::string userNickName = UserProfile::getInstance()->getUserNickName();

	auto nickNameLabel = Label::createWithTTF(userNickName, "fonts/kor_font.ttf", 40.0f);
	auto subLabel = Label::createWithTTF("WELCOME!!!!", "fonts/kor_font.ttf", 20.0f);

	nickNameLabel->setColor(Color3B::BLACK);
	nickNameLabel->setPosition(100, 700);

	subLabel->setPosition(subLabel->getContentSize().width / 2, -subLabel->getContentSize().height / 2);
	subLabel->setColor(Color3B::BLACK);

	nickNameLabel->addChild(subLabel);
	addChild(nickNameLabel);
}
