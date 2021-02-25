#include "MainScene.h"
#include"ui/CocosGUI.h"
#include"AudioEngine.h"
#include"GameScene.h"
#include"GameManager.h"
using namespace ui;
using namespace cocos2d::experimental;

bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//get window size
	const auto size = Director::getInstance()->getVisibleSize();
	//load resource
	loadResource();
	//set background
	auto bgSprite = Sprite::create("media/0.png");
	bgSprite->setPosition(Vec2(size.width / 2, 368));
	this->addChild(bgSprite);
	bgDownSprite = Sprite::create("media/2.png", Rect(0, 0, 288, 112));
	bgDownSprite->setPosition(Vec2(size.width / 2, 56));
	this->addChild(bgDownSprite);
	//game title
	auto titleSprite = Sprite::create("media/16.png");
	titleSprite->setPosition(Vec2(size.width / 2, size.height / 2 + 150));
	this->addChild(titleSprite);
	//rate
	auto rateButton = Button::create("media/4.png");
	rateButton->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(rateButton);
	rateButton->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type)
		{
			auto rate = GameManager::getInstance()->getRate();
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				if (rate > 5)
				{
					GameManager::getInstance()->setRate(0);
				}
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				GameManager::getInstance()->setRate(++rate);
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
			}
		});
	//menu
	auto leftButton = Button::create("media/18.png");
	leftButton->setPosition(Vec2(size.width / 2 - 80, size.height / 2 - 80));
	this->addChild(leftButton);
	leftButton->addTouchEventListener([&](Ref* pSender,Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				log("Begin Scene Transition");
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				Director::getInstance()->pushScene(TransitionFade::create(0.5f,GameScene::createScene()));
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
			}
		});
	auto rightButton = Button::create("media/19.png");
	rightButton->setPosition(Vec2(size.width / 2 + 80, size.height / 2 - 80));
	this->addChild(rightButton);
	rightButton->addTouchEventListener([](Ref* pSender,Widget::TouchEventType type)
		{
		});
	//flybird
	auto flybirdSprite = Sprite::create("media/55.png");
	flybirdSprite->setPosition(Vec2(size.width / 2, size.height / 2 + 60));
	this->addChild(flybirdSprite);
	//down-to-top
	auto moveby = MoveBy::create(0.5f, Vec2(0, 20));
	auto moveUp = EaseCubicActionOut::create(moveby);
	auto moveDown = EaseCubicActionIn::create(moveby->reverse());
	flybirdSprite->runAction(RepeatForever::create(Sequence::create(moveUp, moveDown, nullptr)));
	//animate
	auto flybirdAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("birdfly"));
	flybirdSprite->runAction(flybirdAnimate);
	//update the bottom background
	scheduleUpdate();
	return true;
}

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

void MainScene::update(float)
{
	offset += GameManager::getInstance()->getRate();
	if (offset > 49)
	{
		offset = 0;
	}
	else
	{
		bgDownSprite->setTextureRect(Rect(offset, 0, 288, 112));
	}
}
void MainScene::loadResource()
{
	//create animation
	auto birdAnimation = Animation::create();
	birdAnimation->addSpriteFrame(SpriteFrame::create("media/52.png", Rect(0, 0, 34, 24)));
	birdAnimation->addSpriteFrame(SpriteFrame::create("media/55.png", Rect(0, 0, 34, 24)));
	birdAnimation->addSpriteFrame(SpriteFrame::create("media/58.png", Rect(0, 0, 34, 24)));
	birdAnimation->setDelayPerUnit(0.1f);
	birdAnimation->setRestoreOriginalFrame(true);
	birdAnimation->setLoops(-1);
	AnimationCache::getInstance()->addAnimation(birdAnimation, "birdfly");
	//load music
	AudioEngine::preload("music/sfx_die.wav");
	AudioEngine::preload("music/sfx_hit.wav");
	AudioEngine::preload("music/sfx_swooshing.wav");
	AudioEngine::preload("music/sfx_wing.wav");
}
