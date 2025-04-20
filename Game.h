#ifndef GAME_H
#define GAME_H

#include "Hero.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class Game {
    private:
        Hero hero;
    public:
	Game();
        void newHero(); 
        void loadHero(string); 
        void saveHero(string);
      	void displayHero();
	void valgHero(int);
	~Game();	
};

#endif

