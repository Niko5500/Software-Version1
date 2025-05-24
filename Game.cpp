    #include "Game.h"

    using namespace std;

    Game::Game()
    {
        Grotte* currentGrotte = nullptr;
        int currentEnemyIndex = -1;
    }

    Hero& Game::getHero()
    {
            return hero;
    }


    void Game::setHp(Hero& h)
    {
    hero.setHp(h.getHp());
    }

    void Game::newHero()
    {
        string name;
        int hp = 10;
        int power = 2;
        int level = 1;
        int xp = 0;
        int gold = 0;

        cout << "Enter the name of your Hero: ";
        cin >> name;

        hero = Hero(name, hp, power, level, xp, gold);
    }

    void Game::loadHero(sqlite3* db, int heroId)
    {
        sqlite3_stmt* stmt;
        const char* selectHeroSQL = "SELECT id, name, hp, maxHp, power, level, xp, gold FROM Hero WHERE id = ?";

        if (sqlite3_prepare_v2(db, selectHeroSQL, -1, &stmt, nullptr) != SQLITE_OK)
        {
            cout << "Failed to prepare hero SELECT statement!" << endl;
            return;
        }

        sqlite3_bind_int(stmt, 1, heroId);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int heroId = sqlite3_column_int(stmt, 0);
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int hp = sqlite3_column_int(stmt, 2);
            int maxHp = sqlite3_column_int(stmt, 3);
            int power = sqlite3_column_int(stmt, 4);
            int level = sqlite3_column_int(stmt, 5);
            int xp = sqlite3_column_int(stmt, 6);
            int gold = sqlite3_column_int(stmt, 7);

            hero = Hero(name, hp, power, level, xp, gold);
            sqlite3_finalize(stmt);

            // Læs heroens våben
            const char* selectWeaponsSQL = "SELECT name, power, durability, price FROM Weapon WHERE hero_id = ?";
            if (sqlite3_prepare_v2(db, selectWeaponsSQL, -1, &stmt, nullptr) != SQLITE_OK)
            {
                cout << "Failed to prepare weapon SELECT statement!" << endl;
                return;
            }

            sqlite3_bind_int(stmt, 1, heroId);
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                string wName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                int wPower = sqlite3_column_int(stmt, 1);
                int wDurability = sqlite3_column_int(stmt, 2);
                int wPrice = sqlite3_column_int(stmt, 3);

                Weapon* w = new Weapon(wName, wPower, wDurability, wPrice);
                hero.addWeapon(w);
            }

            sqlite3_finalize(stmt);
            displayHero();
        }
        else
        {
            cout << "No saved hero found with that ID!" << endl;
            sqlite3_finalize(stmt);
        }
    }

    void Game::saveHero(sqlite3* db, Hero& hero)
    {
        sqlite3_stmt* stmt;

        // 1. Check om hero findes (baseret på navn)
        const char* checkHeroSQL = "SELECT id FROM Hero WHERE name = ?";
        int heroId = -1;

        if (sqlite3_prepare_v2(db, checkHeroSQL, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, hero.getName().c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                heroId = sqlite3_column_int(stmt, 0); // Hero findes
            }
            sqlite3_finalize(stmt);
        }

        if (heroId != -1) {
            // 2. Hero findes → UPDATE
            const char* updateSQL = R"(
                UPDATE Hero SET hp = ?, maxHp = ?, power = ?, level = ?, xp = ?, gold = ?
                WHERE id = ?
            )";
            if (sqlite3_prepare_v2(db, updateSQL, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, hero.getHp());
                sqlite3_bind_int(stmt, 2, hero.getHp()); // maxHp
                sqlite3_bind_int(stmt, 3, hero.getPower());
                sqlite3_bind_int(stmt, 4, hero.getLevel());
                sqlite3_bind_int(stmt, 5, hero.getXp());
                sqlite3_bind_int(stmt, 6, hero.getGold());
                sqlite3_bind_int(stmt, 7, heroId);

                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    cout << "Failed to update hero!" << endl;
                }
                sqlite3_finalize(stmt);
            }
        } else {
            // 3. Hero findes IKKE → INSERT
            const char* insertSQL = R"(
                INSERT INTO Hero(name, hp, maxHp, power, level, xp, gold)
                VALUES (?, ?, ?, ?, ?, ?, ?)
            )";
            if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, hero.getName().c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt, 2, hero.getHp());
                sqlite3_bind_int(stmt, 3, hero.getHp()); // maxHp
                sqlite3_bind_int(stmt, 4, hero.getPower());
                sqlite3_bind_int(stmt, 5, hero.getLevel());
                sqlite3_bind_int(stmt, 6, hero.getXp());
                sqlite3_bind_int(stmt, 7, hero.getGold());

                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    cout << "Failed to insert hero!" << endl;
                    sqlite3_finalize(stmt);
                    return;
                }
                heroId = sqlite3_last_insert_rowid(db); // Få ny id
                sqlite3_finalize(stmt);
            }
        }

        // 4. Gem våben (evt. slet gamle først for at undgå dubletter)
        const char* deleteOldWeaponsSQL = "DELETE FROM Weapon WHERE hero_id = ?";
        if (sqlite3_prepare_v2(db, deleteOldWeaponsSQL, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, heroId);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }

        const char* insertWeaponSQL = R"(
            INSERT INTO Weapon(name, power, durability, price, hero_id)
            VALUES (?, ?, ?, ?, ?)
        )";

        vector<Weapon*> inv = hero.getInventory();
        for (Weapon* w : inv) {
            if (sqlite3_prepare_v2(db, insertWeaponSQL, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, w->getName().c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt, 2, w->getPower());
                sqlite3_bind_int(stmt, 3, w->getDurability());
                sqlite3_bind_int(stmt, 4, w->getPrice());
                sqlite3_bind_int(stmt, 5, heroId);

                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    cout << "Failed to insert weapon!" << endl;
                }
                sqlite3_finalize(stmt);
            }
        }

        cout << "Hero and inventory saved to database!" << endl;
    }


    void Game::displayHero()
    {
        cout << "Hero Name: " << hero.getName() << endl;
        cout << "HP: " << hero.getHp() << endl;
        cout << "Power: " << hero.getPower() << endl;
        cout << "Level: " << hero.getLevel() << endl;
        cout << "XP: " << hero.getXp() << endl;
        cout << "Gold: " << hero.getGold() << endl;
        cout << "Inventory: "; hero.displayInventory(); cout << "" << endl;
    }

    void Game::listHeroes(sqlite3* db)
    {
        const char* sql = "SELECT id, name, level, xp, gold FROM Hero";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            cout << "Failed to prepare list heroes statement!" << endl;
            return;
        }

        cout << "Saved heroes in database:" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int level = sqlite3_column_int(stmt, 2);
            int xp = sqlite3_column_int(stmt, 3);
            int gold = sqlite3_column_int(stmt, 4);

            cout << "ID: " << id << ", Name: " << name 
                << ", Level: " << level << ", XP: " << xp 
                << ", Gold: " << gold << endl;
        }

        sqlite3_finalize(stmt);
    }

    void Game::choseHero(int n, sqlite3* db)
    {
        while (n != 0 && n != 1)
        {
            cout << "Incorrect. Type 0 for new hero or 1 to load an existing hero: ";
            cin >> n;
        }

        if (n == 0)
        {
            newHero();
        }
        else
        {
            listHeroes(db);  // Viser heroes i databasen

            int heroId;
            cout << "Type the ID of the hero to load: ";
            cin >> heroId;

            cout << "-----------------------------------" << endl;
            cout << "Hero stats: " << endl;
            cout << "-----------------------------------" << endl;

            loadHero(db, heroId);  // Indlæser hero baseret på ID
        }
    }

 void Game::updateHero_Kills(sqlite3* db, int heroId, int enemyId, int weaponId)
{
    sqlite3_stmt* stmt;
    const char* insertHero_KillsSQL = R"(
        INSERT INTO Hero_Kills(hero_id, enemy_id, weapon_id, kill_time)
        VALUES (?, ?, ?, datetime('now'))
    )";

    if (sqlite3_prepare_v2(db, insertHero_KillsSQL, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cout << "Failed to prepare insert into Hero_Kills!" << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, heroId);
    sqlite3_bind_int(stmt, 2, enemyId);
    sqlite3_bind_int(stmt, 3, weaponId);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        cout << "Failed to insert kill record!" << endl;
    }

    sqlite3_finalize(stmt);
}


    void Game::analyse(sqlite3* db)
    {
        while (true)
        {
            cout << "\nAnalyse menu:\n"
                << "(0) Show heroes\n"
                << "(1) Show enemies defeated for each hero\n"
                << "(2) For a given hero, show weapon kills\n"
                << "(3) For each weapon, show which hero has most kills\n"
                << "(4) Exit analyse\n"
                << "Chose an input: ";

            int input;
            cin >> input;
            cin.ignore();

            cout << "" << endl;

            if (input == 0)
            {
                listHeroes(db);
            }
            else if (input == 1)
            {
                string sql = R"(SELECT Hero.name, COUNT(*) AS kills
                                FROM Hero_Kills
                                JOIN Hero ON Hero_Kills.hero_id = Hero.id
                                GROUP BY Hero.id
                                ORDER BY kills DESC;)";

                sqlite3_stmt* stmt;
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
                {
                    while (sqlite3_step(stmt) == SQLITE_ROW)
                    {
                        string heroName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                        int kills = sqlite3_column_int(stmt, 1);
                        cout << "Hero: " << heroName << ", Kills: " << kills << endl;
                    }
                }
                sqlite3_finalize(stmt);
            }
            else if (input == 2)
            {
                cout << "Enter a Hero id: ";
                string name;
                getline(cin, name);  // bedre end cin >> name, så navne med mellemrum virker også

                string sql = R"(SELECT Weapon.name, COUNT(*) AS kills
                                FROM Hero_Kills
                                JOIN Weapon ON Hero_Kills.weapon_id = Weapon.id
                                JOIN Hero ON Hero_Kills.hero_id = Hero.id
                                WHERE Hero.name = ?
                                GROUP BY Weapon.id
                                ORDER BY kills DESC;)";

                sqlite3_stmt* stmt;
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
                {
                    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

                    while (sqlite3_step(stmt) == SQLITE_ROW)
                    {
                        string weaponName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                        int kills = sqlite3_column_int(stmt, 1);
                        cout << "Weapon: " << weaponName << ", Kills: " << kills << endl;
                    }
                }
                sqlite3_finalize(stmt);
            }
            else if (input == 3)
            {
                string sql = R"(
                    SELECT Weapon.name, Hero.name, MAX(kills) FROM (
                        SELECT Weapon.name AS weapon_name,
                            Hero.name AS hero_name,
                            Weapon.id AS weapon_id,
                            Hero.id AS hero_id,
                            COUNT(*) AS kills
                        FROM Hero_Kills
                        JOIN Weapon ON Hero_Kills.weapon_id = Weapon.id
                        JOIN Hero ON Hero_Kills.hero_id = Hero.id
                        GROUP BY Weapon.id, Hero.id
                    )
                    GROUP BY weapon_id;
                )";

                sqlite3_stmt* stmt;
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
                {
                    while (sqlite3_step(stmt) == SQLITE_ROW)
                    {
                        string weaponName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                        string heroName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                        int kills = sqlite3_column_int(stmt, 2);
                        cout << "Weapon: " << weaponName << ", Top Hero: " << heroName << ", Kills: " << kills << endl;
                    }
                }
                sqlite3_finalize(stmt);
            }
        

            else if (input == 4)
            {
                cout << "exiting analyses" << endl;
                return; 
            }

            else
            {
                cout << "Ugyldigt valg, prøv igen.\n";
            }
        }
    }



    Grotte* Game::chooseGrotte()
    {	
        GrotteFactory grotteFactory;
        Grotte* choosenGrotte = nullptr;
        vector<string> options;
        
        // Hero adgang til grotter i forhold til level
        
            if (hero.getLevel() >= 0)
            {
                options.push_back("Easy");
            }
            if (hero.getLevel() >= 5)
            {
                options.push_back("Medium");
            }
            if (hero.getLevel() >= 10)
            {
                options.push_back("Hard");
            }
            if (hero.getLevel() >= 15)
            {
                options.push_back("Very Hard");
            }
            if (hero.getLevel() >= 25)
            {
                options.push_back("Extreme");
            }

            bool validChoice = false;

            while (!validChoice)
            {
                // Viser grotter 
                cout << "Grotter: " << endl;

                for (int i = 0; i < options.size(); i++)
                {
                    cout << "(" << i<< ") " << options[i] << endl;
                }
                cout << "Choose a grotte: ";
                string valg;
                cin >> valg;

                // Tjek at hele inputtet er tal
                bool isNumber = true;
                for (char c : valg) {
                    if (!isdigit(c)) {
                        isNumber = false;
                        break;
                    }
                }

                if (isNumber)
                {
                    int valgnum = stoi(valg);
                    if (valgnum >= 0 && valgnum < options.size())
                    {
                        choosenGrotte = grotteFactory.createGrotte(options[valgnum]);
                        validChoice = true;
                    }
                    else 
                    {
                        cout << "invalid number, try again: " << endl;
                    
                    }
                }
                else
                {
                    cout << "choose a number again" << endl;
                }
        }

        return choosenGrotte;
    }

    int Game::chooseEnemyIndex(Grotte* choosenGrotte)
    {
        vector<Enemy*>& enemies = choosenGrotte->getEnemies();

        cout << "Enemies in grotten: " << endl;
        cout << "" << endl;

        for (int i = 0; i < enemies.size(); i++) {
            cout << "Enemy: "<< i << endl;
            cout << enemies[i]->getName() << endl;
            cout << "hp: " << enemies[i]->getHp() << endl;
            cout << "power: " << enemies[i]->getPower() << endl;
            cout << "xp: " << enemies[i]->getXp() << endl;
            cout << "" << endl;
        }

        cout << "Choose an enemy to fight: ";

        int valgnum = -1;
        bool validChoice = false;

        while (!validChoice)
        {
            string valg;
            cin >> valg;

            bool isNumber = true;
            for (char c : valg) {
                if (!isdigit(c)) {
                    isNumber = false;
                    break;
                }
            }

            if (isNumber)
            {
                valgnum = stoi(valg);
                if (valgnum >= 0 && valgnum < enemies.size())
                {
                    validChoice = true;
                }
                else 
                {
                    cout << "Ugyldigt tal, prøv igen." << endl;
                }
            }
            else
            {
                cout << "Vælg et tal, prøv igen." << endl;
            }
        }

        return valgnum;
    }

    void Game::deleteCurrentGrotte()
    {
        if (currentGrotte != nullptr)
        {
            delete currentGrotte;
            currentGrotte = nullptr;
        }
    }

    void Game::gameRules()
    {
        cout << "===================================" << endl;
        cout << "             Game rules            " << endl;
        cout << "===================================" << endl;
        cout << "" << endl;
        cout << "1. The goal is to defeat the dragon" << endl;
        cout << " - The dragon is in the Extreme cave" << endl;
        cout << " - The Extreme cave unlocks at lvl 25" << endl;
        cout << "2. Defeat enemies in caves to gain xp and gold" << endl;
        cout << "3. Weapons can be bought in armory or found in caves" << endl;
        cout << "4. When a cave is defeated u can choose a new one" << endl;
        cout << "5. You can exit and save game after defeating a cave" << endl;
        cout << "6. losing a battle will reset hero down to last save" << endl;
    }


    void Game::start(sqlite3* db) // db skal sendes ind som parameter
    {
        srand(time(0)); // Sæt tilfældig seed én gang
        
        Enemy dummyEnemy("Dummy", 1, 1, 1);
        Grotte dummyGrotte("Dummy", 1, {});
        Fight fight(hero, dummyEnemy, dummyGrotte);
        Armory armory(hero);

        while(true)
        {
            int valg;
            cout << "(0) New Game, (1) Load Game, (2) Analyse saves, (3) Exit game: ";
            cin >> valg;    
            cin.ignore();

            if (valg == 0 or valg == 1)
            {
                choseHero(valg, db); // db sendes med som variabel
                break;
            }

            if (valg == 2)
            {
                analyse(db);
            }

            if (valg == 3)
            {
                cout << "exiting game" << endl;
                exit(0);
            }
        }

        gameRules();
        cout << endl;

        bool heroLever = true;
        while (heroLever) 
        {   
            cout << "Your options are: (0) Fight Monsters, (2) Go to Armory, (4) Save and Exit: ";
            int input;
            cin >> input;
            cout << endl;

            if (input == 0) 
            {
                currentGrotte = chooseGrotte();
                fight.setGrotte(*currentGrotte);
                cout << endl;

                // Bekæmp alle fjender i grotten én efter én
                while (!currentGrotte->isEmpty())
                {
                    currentEnemyIndex = chooseEnemyIndex(currentGrotte); 
                    Enemy chosenEnemy = *currentGrotte->getEnemies()[currentEnemyIndex];
                    fight.setEnemy(chosenEnemy);
                    cout << "-----------------------------------" << endl;
                    cout << "Stats for Hero and Enemy" << endl;

                    fight.printHero();
                    hero.printWeapon();
                    fight.printEnemy();

                    cin.ignore();

                    fight.runFight();

                    updateHero_Kills(db, hero.getId(), chosenEnemy.getId(), hero.getWeaponId());

                    currentGrotte->removeEnemy(currentEnemyIndex);
                }
                cout << endl << "You have cleared Grotten" << endl;
                fight.setHeroGold();
                cout << "Hero gained gold: " << currentGrotte->getGold() << endl;
                deleteCurrentGrotte();
            }
            else if (input == 2) 
            {
                cout << "<><><><><><><><><><><><><><><><><><><><><>" << endl;
                cout << "Welcome to the Armory" << endl;
                cout << "You can buy weapons here" << endl << endl;

                bool inArmory = true;
                armory.loadWeapons();
                armory.displayWeapons();

                while (inArmory)
                {
                    cout << "--------------------------------------------" << endl;
                    armory.enterArmory();
                    cout << endl;
                    cout << "Your options: (0) Buy Weapons, (1) Equip Weapon, (2) Unequip Weapon, (3) Exit Armory: ";
                    char choose;
                    cin >> choose;

                    if (choose == '0')
                    {
                        armory.buyWeapon();
                        cout << endl;
                    }
                    else if (choose == '1')
                    {
                        cout << endl;
                        hero.displayInventory();
                        cout << endl;
                        cout << "Choose a weapon to equip: ";
                        int weaponIndex;
                        cin >> weaponIndex;
                        cout << endl;
                        hero.equipWeapon(weaponIndex);
                    }
                    else if (choose == '2')
                    {
                        cout << "Unequipped "; 
                        hero.printWeapon();
                        cout << endl;
                        hero.unequipWeapon();
                    }
                    else if (choose == '3')
                    {
                        cout << "Exiting armory" << endl;
                        inArmory = false;
                    }
                }
            }
            else if (input == 4) // Save and exit
            {
                saveHero(db, hero);
                deleteCurrentGrotte();
                exit(0);
            }
            
        }

        deleteCurrentGrotte(); // Slet grotten hvis hero dør
    }


    Game::~Game(){}