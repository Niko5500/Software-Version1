#ifndef GAME_H
#define GAME_H

#include "Hero.h"
#include "Enemy.h"
#include "Fight.h"
#include "EnemyFactory.h"
#include "GrotteFactory.h"
#include "Armory.h"
#include "Weapon.h"
#include "WeaponFactory.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>  // til at arbejde med mapper og filer
#include <sqlite3.h>

using namespace std;

class Game {
    protected:
        Hero hero;
		Grotte* currentGrotte = nullptr;
		int currentEnemyIndex = -1;

    public:
	Game();
	static bool isNumber(const std::string& s);
	Hero& getHero();
	void setHp(Hero& h);
    void newHero(); 
    void loadHero(sqlite3* db, int heroId); 
    void saveHero(sqlite3* db, Hero& hero);
	void deleteHero(sqlite3* db, int heroId);
	void listHeroes(sqlite3* db);
    void displayHero();
	void choseHero(int input, sqlite3* db);
	bool heroExists(sqlite3* db, int heroId);
	void updateHero_Kills(sqlite3* db, int heroId, int enemyId, int weaponId);
	void analyse(sqlite3* db);
	Grotte* chooseGrotte();
	int chooseEnemyIndex(Grotte* choosenGrotte);
	void deleteCurrentGrotte();
	void gameRules();
	void start(sqlite3* db);

	~Game();
};

#endif
