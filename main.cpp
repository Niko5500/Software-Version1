#include "Hero.h"
#include "Enemy.h"
#include "Game.h"
#include "Fight.h"
#include <iostream>
#include <sqlite3.h>

int main()
{
    sqlite3* db;
    int rc = sqlite3_open("myDataBase.db", &db);  // åbner (eller opretter) databasefilen "game.db"

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    Game game;
    game.start(db);  // kalder start med db

    sqlite3_close(db);  // luk databasen, når programmet slutter
    return 0;
}
