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
	Hero& getHero();
	void setHp(Hero& h);
        void newHero(); 
        void loadHero(string); 
        void saveHero(string);
      	void displayHero();
	void valgHero(int);
	void gameMenu(Fight& fight);
	void start();

	~Game();	
};

#endif
