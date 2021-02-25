#include "GameScene.h"
#include<vector>

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//
	_size = Director::getInstance()->getWinSize();
	//set background
	auto bgSprite = Sprite::create("media/1.png");
	bgSprite->setPosition(Vec2(_size.width / 2, 368));
	this->addChild(bgSprite);
	bgDownSprite = Sprite::create("media/2.png", Rect(0, 0, 288, 112));
	bgDownSprite->setPosition(Vec2(_size.width / 2, 56));
	//
	auto bgDownBody = PhysicsBody::createBox(Size(288, 112));
	bgDownBody->setDynamic(false);
	//类型1
	bgDownBody->setCategoryBitmask(1);
	bgDownBody->setContactTestBitmask(2);
	bgDownBody->setGroup(1);
	bgDownSprite->setPhysicsBody(bgDownBody);
	this->addChild(bgDownSprite);
	//title sprite
	auto title1Sprite = Sprite::create("media/10.png");
	title1Sprite->setPosition(Vec2(_size.width / 2 - 50, _size.height / 2 + 150));
	vectors.pushBack(title1Sprite);
	this->addChild(title1Sprite);
	auto title2Sprite = Sprite::create("media/11.png");
	title2Sprite->setPosition(Vec2(_size.width / 2 + 50, _size.height / 2 + 148));
	vectors.pushBack(title2Sprite);
	this->addChild(title2Sprite);
	//tap sprite
	auto tap1Sprite = Sprite::create("media/20.png");
	tap1Sprite->setPosition(Vec2(_size.width / 2 - 40, _size.height / 2));
	vectors.pushBack(tap1Sprite);
	this->addChild(tap1Sprite);
	auto tap2Sprite = Sprite::create("media/21.png");
	tap2Sprite->setPosition(Vec2(_size.width / 2 + 40, _size.height / 2));
	vectors.pushBack(tap2Sprite);
	this->addChild(tap2Sprite);
	//finger
	auto fingerSprite = Sprite::create("media/22.png");
	fingerSprite->setPosition(_size / 2);
	vectors.pushBack(fingerSprite);
	this->addChild(fingerSprite);
	auto blink = Blink::create(4.0f, 5);
	fingerSprite->runAction(RepeatForever::create(blink));
	//up point
	auto upPointSprite = Sprite::create("media/17.png");
	upPointSprite->setPosition(Vec2(_size.width / 2, _size.height / 2 + 40));
	vectors.pushBack(upPointSprite);
	this->addChild(upPointSprite);
	//undisable bird
	auto unBirdSprite = Sprite::create("media/15.png");
	unBirdSprite->setPosition(Vec2(_size.width / 2, _size.height / 2 + 80));
	vectors.pushBack(unBirdSprite);
	this->addChild(unBirdSprite);
	//add clipnode
	auto clipNode = ClippingNode::create();
	auto stencil = Sprite::create("media/22.png");
	auto clipLayer = LayerColor::create(Color4B(Color3B::BLACK, 150));
	clipNode->setContentSize(Size(288, 624));
	clipNode->addChild(clipLayer);
	clipNode->setAlphaThreshold(0.05f);
	clipNode->setStencil(stencil);
	clipNode->setAnchorPoint(Vec2(0, 0));
	clipNode->setPosition(Vec2(0, 0));
	clipNode->setInverted(true);
	stencil->setPosition(fingerSprite->getPosition());
	clipNode->setTag(1);
	this->addChild(clipNode);
	//listener
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//
	auto keyPressListener = EventListenerKeyboard::create();
	keyPressListener->onKeyPressed = [&](EventKeyboard::KeyCode kecycode, Event* event)
	{
		if (kecycode == EventKeyboard::KeyCode::KEY_W)
		{
			_player->getPhysicsBody()->applyImpulse(Vec2(0, 2000));
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyPressListener, this);
	//contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [&, listener](PhysicsContact& contact)
	{
		if (!isEnd)
		{
			//取消碰撞组
			contact.getShapeA()->getBody()->setContactTestBitmask(0);
			contact.getShapeB()->getBody()->setContactTestBitmask(1);
			unscheduleAllCallbacks();
			_player->stopAllActions();
			_eventDispatcher->removeEventListener(listener);
			fail();
			drawScore(0);
			isEnd = true;
		}
		return true;
	};
	contactListener->onContactSeparate = [&](PhysicsContact& contact)
	{

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	schedule(CC_SCHEDULE_SELECTOR(GameScene::rubRaise), _rate);
	schedule(CC_SCHEDULE_SELECTOR(GameScene::rubMove));
	schedule(CC_SCHEDULE_SELECTOR(GameScene::drawScore));
	scheduleUpdate();
	return true;
}

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, -300));
	scene->getPhysicsWorld()->setAutoStep(true);
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}
void GameScene::update(float)
{
	offset += _rate;
	if (offset > 49)
	{
		offset = 0;
	}
	else
	{
		bgDownSprite->setTextureRect(Rect(offset, 0, 288, 112));
	}
}

bool GameScene::onTouchBegan(Touch*, Event*)
{
	if (logic == Logic::PreStart)
	{
		this->removeChildByTag(1);
		for (auto child : vectors)
		{
			this->removeChild(child);
		}
		logic = Logic::Start;
		//init the palyer
		const auto physicsMaterial = PhysicsMaterial(0, 0, 0);
		const auto physicsBody = PhysicsBody::createBox(Size(34, 24), physicsMaterial);
		physicsBody->setGroup(1);
		physicsBody->setContactTestBitmask(2);
		physicsBody->setCategoryBitmask(3);
		physicsBody->setRotationEnable(false);
		_player = Sprite::create("media/55.png");
		_player->setPhysicsBody(physicsBody);
		_player->getPhysicsBody()->setDynamic(true);
		_player->setPosition(Vec2(_size.width / 2, _size.height / 2 + 80));
		this->addChild(_player);
		_player->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("birdfly")));
	}
	else
	{
		_player->getPhysicsBody()->applyImpulse(Vec2(0, 100));
	}
	return true;
}

void GameScene::onTouchMoved(Touch*, Event*)
{
}

void GameScene::onTouchEnded(Touch*, Event*)
{
}

void GameScene::updatePhysicsWorld(float dt)
{
	for (int i = 0; i < 3; i++)
	{
		static_cast<Scene*>(this->getParent())->getPhysicsWorld()->step(1 / 180.0f);
	}
}

void GameScene::fail()
{
	//Title Sprite
	auto leftTitle = Sprite::create("media/12.png");
	leftTitle->setPosition(Vec2(_size.width / 2 - 50, _size.height + 50));
	this->addChild(leftTitle);
	auto rightTitle = Sprite::create("media/13.png");
	rightTitle->setPosition(Vec2(_size.width / 2 + 50, _size.height + 50));
	this->addChild(rightTitle);
	auto moveby = MoveBy::create(0.5f, Vec2(0, -50 - _size.height / 2 + 150));
	leftTitle->runAction(moveby);
	rightTitle->runAction(moveby->clone());
	//Score Board Sprite
	auto scoreBoard = Sprite::create("media/38.png");
	scoreBoard->setPosition(Vec2(_size.width / 2, -60));
	this->addChild(scoreBoard);
	auto moveto = MoveTo::create(0.5f, Vec2(_size.width / 2, _size.height / 2 + 50));
	//model
	auto model = Sprite::create("media/39.png");
	model->setPosition(Vec2(48, 52));
	scoreBoard->addChild(model);
	scoreBoard->runAction(moveto);

}

void GameScene::drawScore(float dt)
{
	if (dt == 0)
	{
		for (int i = 1; i <= score; i++)
		{
			//求出i值的长度，即几位数
			int num = 10;
			while (i % num != i)
			{
				num *= 10;
			}
			int len = log10(num);
			//根据位数创建vector及对该数进行分离
			std::vector<int> split_value;
			int temp = i;
			len--;
			while (len >= 1)
			{
				int value = temp / pow(10, len);
				split_value.push_back(value);
				temp -= pow(10, len) * value;
				len--;
			}
			split_value.push_back(temp);
			//绘制分数
			int x = 144;
			int y = 362;
			for (int j = split_value.size() - 1; j >= 0; j--)
			{
				char name[20];
				sprintf(name, "score/%d.png", split_value.at(j));
				auto scoreSprite = Sprite::create(name);
				scoreSprite->setPosition(Vec2(x, y));
				this->addChild(scoreSprite);
				x -= 12;
			}
		}
	}

}

void GameScene::rubRaise(float)
{
	if (logic == Logic::Start)
	{
		//change the anchor to (0.5,1)
		int y1 = random() % 271 + 50;
		auto downRub = Sprite::create("media/45.png", Rect(0, 0, 52, y1));
		downRub->setTag(1);
		downRub->setAnchorPoint(Vec2(0.5, 1));
		downRub->setPosition(Vec2(414, y1 + 112));
		//add physcisbody
		auto rubBody1 = PhysicsBody::createBox(Size(52, y1), PhysicsMaterial());
		rubBody1->setCategoryBitmask(2);
		rubBody1->setGroup(1);
		rubBody1->setContactTestBitmask(2);
		rubBody1->setDynamic(false);
		downRub->setPhysicsBody(rubBody1);
		this->addChild(downRub);
		//change the anchor to (0.5,0)
		int y2 = 522 - 180 - y1;
		auto topRub = Sprite::create("media/44.png", Rect(0, 320 - y2, 52, y2));
		topRub->setTag(1);
		topRub->setAnchorPoint(Vec2(0.5, 0));
		topRub->setPosition(Vec2(414, y1 + 292));
		//add physcisbody
		auto rubBody2 = PhysicsBody::createBox(Size(52, y2), PhysicsMaterial());
		rubBody2->setCategoryBitmask(2);
		rubBody2->setDynamic(false);
		rubBody2->setGroup(1);
		rubBody2->setContactTestBitmask(2);
		topRub->setPhysicsBody(rubBody2);
		this->addChild(topRub);
		//add
		rubVectors.pushBack(downRub);
		rubVectors.pushBack(topRub);
	}
}

void GameScene::rubMove(float)
{
	if (!rubVectors.empty())
	{
		Vector<Sprite*> removeVector;
		for (auto& v : rubVectors)
		{
			if (v->getPositionX() <= -26)
			{
				removeVector.pushBack(v);
			}
			else if (v->getPositionX() > _player->getPositionX() && v->getTag() == 1)
			{
				score++;
				v->setTag(2);
			}
			else
			{
				v->setPositionX(v->getPositionX() - _rate * 2.5);
			}
		}
		if (!removeVector.empty())
		{
			for (auto& v : removeVector)
			{
				rubVectors.eraseObject(v);
				this->removeChild(v);
			}
		}
	}
}
