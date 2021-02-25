#include"GameManager.h"

static GameManager* gameManager=nullptr;
GameManager* GameManager::getInstance()
{
	if (!gameManager)
	{
		gameManager = new GameManager;
		return gameManager;
	}
	return gameManager;
}

int GameManager::getRate()
{
	return _rate;
}

void GameManager::setRate(int rate)
{
	_rate = rate;
}
