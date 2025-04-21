#ifndef GAME_H
#define GAME_H

#include "Hero.h"
#include "Enemy.h"
#include "Fight.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Game {
    protected:
        Hero hero;
    public:
	Game();
        void newHero(); 
        void loadHero(string); 
        void saveHero(string);
      	void displayHero();
	void valgHero(int);
	Hero& getHero();
	void setHp(Hero& h);
	void gameMenu(Fight& fight);

	~Game();	
};

#endif

