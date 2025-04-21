#ifndef FIGHT_H
#define FIGHT_H

#include "Hero.h"
#include "Enemy.h"
#include "iostream"
#include "vector"

class Fight {
	protected:
	       Hero& hero;
	       Enemy& enemy;
	       vector <Enemy> enemies;

	public:
	        Fight(Hero& h, Enemy& e);
		Hero& getHeroFight();
 		Hero& getHero();
		void printHero();
		Hero& setHeroXp();
		Hero& setHeroLevel();
		Enemy& getEnemyFight();
		Enemy& getEnemy();
		void printEnemy();
		Enemy& chooseEnemy();
				
		~Fight();
};

#endif
