#ifndef HERO_H
#define HERO_H
#include <string>
using namespace std;

class Hero {
	protected:
		string name;
		int hp;
		int maxHp;
		int power;
		int level;
		int xp;
	public:
		Hero();
		Hero(string, int, int, int, int);
		
		string getName();

		int getXp();
		void setXp(int newXp);

		int getLevel();
		void setLevel(int newLevel);

		int getHp();
		void setHp(int newHp);
		int getMaxHp();
		void setMaxHp(int newMaxHp);
				
		int getPower();
		void setPower(int newPower);	

		~Hero();

};

#endif
