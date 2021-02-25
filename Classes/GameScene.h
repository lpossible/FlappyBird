#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_
#include"cocos2d.h"
#include"GameManager.h"
USING_NS_CC;
class GameScene:public Layer
{
	enum class Logic
	{
		PreStart,
		Start,
	};
public:
	bool init()override;
	static Scene* createScene();
	CREATE_FUNC(GameScene);
private:
	int score = 0;
	Logic logic = Logic::PreStart;
	Sprite* bgDownSprite;
	int offset = 0;
	Size _size;
	Sprite* _player;
	Vector<Sprite*> vectors;
	Vector<Sprite*> rubVectors;
	void update(float)override;
	bool onTouchBegan(Touch*, Event*)override;
	void onTouchMoved(Touch*,Event*)override;
	void onTouchEnded(Touch*,Event*)override;
	void updatePhysicsWorld(float);
	void fail();
	void drawScore(float);
	void rubRaise(float);
	void rubMove(float);
	int  _rate = GameManager::getInstance()->getRate();
	bool isEnd = false;

};
#endif // !GAME_SCENE_H_
