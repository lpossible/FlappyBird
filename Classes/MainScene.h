#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_
#include"cocos2d.h"
USING_NS_CC;
class MainScene:public Layer
{
public:
	bool init()override;
	static Scene* createScene();
	CREATE_FUNC(MainScene);
private:
	void update(float)override;
	Sprite* bgDownSprite;
	int offset = 0;
	void loadResource();
};
#endif // !MAIN_SCENE_H_

