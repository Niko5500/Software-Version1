#ifndef GAME_H
#define GAME_H

#include "Hero.h"
#include "Enemy.h"
#include "Fight.h"
#include "EnemyFactory.h"
#include "GrotteFactory.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>  // til at arbejde med mapper og filer

using namespace std;

class Game {
    protected:
        Hero hero;
		Grotte* currentGrotte = nullptr;
		int currentEnemyIndex = -1;

    public:
	Game();
	Hero& getHero();
	void setHp(Hero& h);
    void newHero(); 
    void loadHero(string); 
    void saveHero(string);
	void listTxtFiles();
    void displayHero();
	void valgHero(int);
	Grotte* chooseGrotte();
	int chooseEnemyIndex(Grotte* choosenGrotte);
	void deleteCurrentGrotte();
	void gameRules();
	void start();

	~Game();
};

#endif
