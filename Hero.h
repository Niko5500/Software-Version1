#ifndef HERO_H
#define HERO_H
#include <string>
using namespace std;

class Hero {
	protected:
		string name;
		int hp;
		int power;
		int level;
		int xp;
	public:
		Hero();
		Hero(string, int, int, int, int);
		string getName();
		int getXp();
		int getLevel();
		int getHp();
		int getPower();

		~Hero();
};

#endif
