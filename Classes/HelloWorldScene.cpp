#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	refreshQuestion();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto sprite = Sprite::create("game_background.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
	btn_so = Button::create("btn_so.png");
	btn_so->setPosition(Vec2(130, 150));
	btn_so->setZoomScale(0.7f);
	btn_so->addTouchEventListener([&](Ref*,Widget::TouchEventType e) {
		if (e == Widget::TouchEventType::BEGAN)
		{
			SimpleAudioEngine::getInstance()->playEffect("sound/so.ogg");
			correctCheck(0);
		}
	});
	addChild(btn_so);

	btn_ma = Button::create("btn_ma.png");
	btn_ma->setPosition(Vec2(350, 150));
	btn_ma->setZoomScale(0.7f); 
	btn_ma->addTouchEventListener([&](Ref*, Widget::TouchEventType e) {
		if (e == Widget::TouchEventType::BEGAN)
		{
			SimpleAudioEngine::getInstance()->playEffect("sound/ma.ogg");
			correctCheck(1);
		}
	});

	addChild(btn_ma);

	swagManager = SomaWordManager::create();
	swagManager->retain();

	timer = ProgressTimer::create(Sprite::create("timer.png"));

	timer->setType(ProgressTimer::Type::BAR);
	timer->setBarChangeRate(Vec2(1, 0));
	timer->setMidpoint(Vec2(0, 1));
	timer->setPosition(visibleSize.width*0.5, 700);

	timer->stopAllActions();

	addChild(timer);

	//score view
	score_label = Label::createWithTTF("0", "fonts/font.ttf", 50.0f,Size::ZERO);

	score_label->setColor(Color3B::BLACK);
	score_label->setPosition(visibleSize.width / 2+70, 750);

	addChild(score_label);

	score = 0;

	somaWordViewer = SomaWordViewer::create();
	
	addChild(somaWordViewer);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/background.wav", true);
	scheduleUpdate();

    return true;
}

void HelloWorld::setTimerPercent(float percent)
{
	timer->stopAllActions();

	timer->setPercentage(percent);

	auto *progressToZero = ProgressFromTo::create(fTime*(percent / 100.0f), percent, 0);

	timer->runAction(progressToZero);
}

void HelloWorld::changeTimerType(int timerType)
{
	if (timerType == 0)
	{
		timer->setTag(0);
		timer->setColor(Color3B::GREEN);
		fTime = swagManager->getCurrentWordShowTime();
	}
	else {
		timer->setTag(1);
		timer->setColor(Color3B::RED);
		fTime = swagManager->getCurrentWaitTime();
	}
}

void HelloWorld::refreshQuestion()
{
	changeTimerType(0);
	setTimerPercent(100.0f);

	swagManager->refresh();

	somaWordViewer->printWords(swagManager->getCurrentQuestion(), swagManager->getCurrentLength());

	btn_so->setVisible(false);
	btn_ma->setVisible(false);
}

void HelloWorld::update(float dt)
{
	if (timer->getPercentage() <= 0.0f)
	{
		//보여주는 타이머 상태
		if (timer->getTag() == 0)
		{
			changeTimerType(1);
			setTimerPercent(100.0f);

			somaWordViewer->hideAllWords();

			btn_so->setVisible(true);
			btn_ma->setVisible(true);
		}
		else if (timer->getTag() == 1) // 맞춰야 하는 상태
		{
			gameOver();
			unscheduleUpdate();
		}
	}
}

void HelloWorld::gameOver()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto overBox = Sprite::create("game_over/box.png");

	overBox->setPosition(visibleSize.width / 2, visibleSize.height + overBox->getContentSize().height / 2);

	auto btnOk = Button::create("game_over/ok_button.png");
	auto retryBtn = Button::create("game_over/retry_button.png");

	btnOk->setPosition(Vec2(110, 55));
	btnOk->addTouchEventListener([&](Ref*, Widget::TouchEventType e) {
		if (e == Widget::TouchEventType::ENDED)
		{
			auto scene = TransitionPageTurn::create(1.0f, MainScene::createScene(), true);
			Director::getInstance()->replaceScene(scene);
		}
	});
	
	retryBtn->setPosition(Vec2(300, 55));

	retryBtn->addTouchEventListener([&](Ref*, Widget::TouchEventType e) {
		if (e == Widget::TouchEventType::ENDED)
		{
			auto scene = TransitionFlipX::create(1.0f, HelloWorld::createScene(),
				TransitionScene::Orientation::RIGHT_OVER);
			Director::getInstance()->replaceScene(scene);
		}
	});

	overBox->addChild(btnOk);
	overBox->addChild(retryBtn);

	auto score_lb = Label::createWithTTF(score_label->getString(), "fonts/font.ttf", 50.0f, Size::ZERO);

	score_lb->setPosition(262, 235);
	overBox->addChild(score_lb);
	addChild(overBox);

	auto actToCenter = MoveTo::create(1.0f, Vec2(visibleSize.width / 2, visibleSize.height/2));
	auto bezier = EaseBounceOut::create(actToCenter);

	overBox->runAction(bezier);
	////

	btn_so->setVisible(false);
	btn_ma->setVisible(false);

	swagManager->release();
	this->pause();
}

void HelloWorld::correctCheck(int type)
{
	if (timer->getTag() == 1)
	{
		if (swagManager->isCorrect(type))
		{
			correctWord();
			somaWordViewer->showWord();

			if (swagManager->isAllCorrect())
			{
				addScore(100);
				refreshQuestion();
			}
		}
		else
			uncorrectWord();
	}
}

void HelloWorld::correctWord()
{
	std::string path = "awesome/" + StringUtils::format("%d", random(1, 9)) + ".png";
	imgEffect(path);
}

void HelloWorld::uncorrectWord()
{
	std::string path = "bad/" + StringUtils::format("%d", random(1, 14)) + ".png";
	imgEffect(path);
	imgEffect("bad_screen.png");

	Device::vibrate(0.1f);
	setTimerPercent(timer->getPercentage() - 30.0f);
}

void HelloWorld::imgEffect(std::string path)
{
	auto corSpr = Sprite::create(path);
	Size visibleSize = Director::getInstance()->getVisibleSize();

	corSpr->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	auto removeAction = CallFuncN::create([&](Node* n) {
		n->removeFromParent();
	});

	auto action = Spawn::createWithTwoActions(ScaleTo::create(0.5f, 2.0f), FadeOut::create(0.5f));
	auto seqAction = Sequence::createWithTwoActions(action, removeAction);

	corSpr->runAction(seqAction);
	addChild(corSpr);
}

void HelloWorld::addScore(int delta)
{
	score += delta;
	score_label->setString(StringUtils::format("%d", score));
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
