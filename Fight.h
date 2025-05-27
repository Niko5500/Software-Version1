#ifndef FIGHT_H
#define FIGHT_H

#include "Hero.h"
#include "Enemy.h"
#include "iostream"
#include "vector"
#include "Grotte.h"

class Fight {
	protected:
	       Hero& hero;
	       Enemy& enemy;
		   Grotte& grotte;

	public:
	    Fight(Hero& h, Enemy& e, Grotte& g);

		Hero& updateHeroFight();
 		Hero& getHero();
		void printHero();
		Hero& setHeroXp();
		Hero& setHeroGold();
		Hero& updateHero();
		void setGrotte(const Grotte& newGrotte);
		Grotte& getGrotte();
		void setEnemy(const Enemy& newEnemy);

		Enemy& updateEnemyFight();
		Enemy& getEnemy();
		void printEnemy();
		Enemy& chooseEnemy();
		bool runFight();
				
		~Fight();
};

#endif
