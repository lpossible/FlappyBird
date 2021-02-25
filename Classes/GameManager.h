#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_
class GameManager
{
private:
	int _rate;
	GameManager():_rate(1) {};
public:
	static GameManager* getInstance();
	int getRate();
	void setRate(int);
};
#endif // !GAME_MANAGER_H_

