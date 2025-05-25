#include "Game.h"

using namespace std;

Game::Game()
    {
        Grotte* currentGrotte = nullptr;
        int currentEnemyIndex = -1;
    }

bool Game::isNumber(const std::string& s) 
{
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cout << "" << endl;

        hero = Hero(-1, name, hp, power, level, xp, gold);
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

            hero = Hero(heroId, name, hp, power, level, xp, gold);
            sqlite3_finalize(stmt);

            // Læs heroens våben
            const char* selectWeaponsSQL = "SELECT id, name, power, durability, price FROM Weapon WHERE hero_id = ?";
            if (sqlite3_prepare_v2(db, selectWeaponsSQL, -1, &stmt, nullptr) != SQLITE_OK)
            {
                cout << "Failed to prepare weapon SELECT statement!" << endl;
                return;
            }

            sqlite3_bind_int(stmt, 1, heroId);
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                int wId = sqlite3_column_int(stmt, 0);
                string wName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                int wPower = sqlite3_column_int(stmt, 2);
                int wDurability = sqlite3_column_int(stmt, 3);
                int wPrice = sqlite3_column_int(stmt, 4);

                Weapon* w = new Weapon(wId ,wName, wPower, wDurability, wPrice);
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

    // 4. Gem våben (slet gamle først)
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
            } else {
                // 💡 Sæt ID direkte på våbenobjektet
                int weaponId = sqlite3_last_insert_rowid(db);
                w->setId(weaponId);
            }

            sqlite3_finalize(stmt);
        }
    }
}

void Game::deleteHero(sqlite3* db, int heroId)
{
    // Slet alle Hero_Kills tilknyttet hero
    string sql1 = "DELETE FROM Hero_Kills WHERE hero_id = ?;";
    sqlite3_stmt* stmt1;
    if (sqlite3_prepare_v2(db, sql1.c_str(), -1, &stmt1, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt1, 1, heroId);
        sqlite3_step(stmt1);
    }
    sqlite3_finalize(stmt1);

    // Slet hero fra Hero-tabellen
    string sql2 = "DELETE FROM Hero WHERE id = ?;";
    sqlite3_stmt* stmt2;
    if (sqlite3_prepare_v2(db, sql2.c_str(), -1, &stmt2, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt2, 1, heroId);
        sqlite3_step(stmt2);
        cout << "Hero with ID " << heroId << " deleted successfully." << endl;
    }
    else
    {
        cout << "Failed to prepare delete statement." << endl;
    }
    sqlite3_finalize(stmt2);
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

void Game::choseHero(int input, sqlite3* db)
{
 
    if (input == 0)
    {   
        cout << "" << endl;
        newHero();
        saveHero(db, hero);
    }
    else if (input == 1)
    {   
        cout << "" << endl;
        listHeroes(db);  // Vis hero-liste

        while (true)
        {
            cout << "Type the ID of the hero to load: ";
            string heroInput;
            getline(cin, heroInput);

            if (!isNumber(heroInput))
            {
                cout << "Invalid Hero ID. Hero ID is a number ." << endl;
                continue;
            }

            int heroId = stoi(heroInput);

            if (heroExists(db, heroId))
            {
                cout << "-----------------------------------" << endl;
                cout << "Hero stats: " << endl;
                cout << "-----------------------------------" << endl;

                loadHero(db, heroId);
                break;
            }
            else
            {
                cout << "Hero with ID " << heroInput << " does not exist." << endl;
            }
        }
    }
    else
    {
        cout << "Invalid choice. Please enter 0 or 1." << endl;
    }
}


bool Game::heroExists(sqlite3* db, int heroId)
{
    sqlite3_stmt* stmt;
    string query = "SELECT COUNT(*) FROM hero WHERE id = ?";

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(stmt, 1, heroId);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return exists;
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
             << "Choose an input: ";

        string inputStr;
        getline(cin, inputStr);

        if (!Game::isNumber(inputStr)) {
            cout << "Invalid input. Write a number.\n";
            continue;
        }

        int input = stoi(inputStr);
        cout << endl;

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
            string heroInput;
            getline(cin, heroInput);

            if (!Game::isNumber(heroInput)) {
                cout << "Invalid Hero ID. Enter a number." << endl;
                continue;
            }

            int heroId = stoi(heroInput);

            if (heroExists(db, heroId))
            {

            string sql = R"(SELECT Weapon.name, COUNT(*) AS kills
                            FROM Hero_Kills
                            JOIN Weapon ON Hero_Kills.weapon_id = Weapon.id
                            JOIN Hero ON Hero_Kills.hero_id = Hero.id
                            WHERE Hero.id = ?
                            GROUP BY Weapon.id
                            ORDER BY kills DESC;)";

            sqlite3_stmt* stmt;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
            {
                sqlite3_bind_int(stmt, 1, heroId);

                while (sqlite3_step(stmt) == SQLITE_ROW)
                {
                    string weaponName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                    int kills = sqlite3_column_int(stmt, 1);
                    cout << "Weapon: " << weaponName << ", Kills: " << kills << endl;
                }
            }
            sqlite3_finalize(stmt);
        }
            else
            {
                cout << "Hero with ID " << heroInput << " does not exist." << endl;
            }
        }
        else if (input == 3)
        {
            string sql = R"(
                SELECT w.name AS weapon_name, h.name AS hero_name, COUNT(*) AS kills
                FROM Hero_Kills hk
                JOIN Hero h ON hk.hero_id = h.id
                JOIN Weapon w ON hk.weapon_id = w.id
                GROUP BY hk.weapon_id, hk.hero_id
                HAVING COUNT(*) = (
                    SELECT MAX(k_count) FROM (
                        SELECT COUNT(*) AS k_count
                        FROM Hero_Kills
                        WHERE weapon_id = hk.weapon_id
                        GROUP BY hero_id
                    )
                )
                ORDER BY w.name;
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
            cout << "Exiting analyse..." << endl;
            return;
        }
        else
        {
            cout << "Invalid input, try again.\n";
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
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

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
                        cout << "invalid number, try again" << endl;
                        cout << "" << endl;
                    
                    }
                }
                else
                {
                    cout << "choose a number again" << endl;
                    cout << "" << endl;
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

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
                    cout << "No grotte found, try again: ";
                }
            }
            else
            {
                cout << "Enter a number, try again: ";
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
        
        Enemy dummyEnemy(1, "Dummy", 1, 1, 1);
        Grotte dummyGrotte("Dummy", 1, {});
        Fight fight(hero, dummyEnemy, dummyGrotte);
        Armory armory(hero);

   while (true)
    {
    string input;
    cout << "(0) New Game, (1) Load Game, (2) Analyse saves, (3) Delete hero, (4) Exit game: ";
    getline(cin, input);

    if (Game::isNumber(input))
    {
        int valg = stoi(input);

        if (valg == 0 || valg == 1)
        {
            choseHero(valg, db);
            break;
        }
        else if (valg == 2)
        {
            analyse(db);
        }
        else if (valg == 3)
        {
            listHeroes(db);
            cout << "Type the ID of the hero to delete: ";
            string heroInput;
            getline(cin, heroInput);

            if (Game::isNumber(heroInput))
            {
                int heroId = stoi(heroInput);

                // Tjek om hero med ID eksisterer
                if (heroExists(db, heroId))
                {
                    deleteHero(db, heroId);
                    cout << "Hero deleted." << endl;
                }
                else
                {
                    cout << "No hero found with ID " << heroId << "." << endl;
                }
            }
            else
            {
                cout << "Invalid hero ID. Must be a number." << endl;
            }
        }

        else if (valg == 4)
        {
            cout << "Exiting game." << endl;
            exit(0);
        }
        else
        {
            cout << "Please enter a number between 0 and 4." << endl;
        }
    }
    else
    {
        cout << "Invalid input, please enter a number." << endl;
    }
}
        gameRules();
        cout << endl;

        bool heroLever = true;
        while (heroLever) 
        {   
            string input1;
            while (true)
            {   
                cout << "Your options are: (0) Fight Monsters, (2) Go to Armory, (4) Save and Exit: ";
                getline(cin, input1);
                    
                if (isNumber(input1))
                {
                    break;
                }

                else
                {
                    cout << "Invalid input, please enter a number. " << endl;
                }
            }
            int input = stoi(input1); 
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

                    fight.runFight();

                    saveHero(db, hero);
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
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

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
                        string input;
                        while(true)
                        {   
                            cout << "Choose a weapon to equip: ";
                            getline(cin, input);
                            if (isNumber(input))
                            {
                                break;
                            }
                        }
                        int weaponIndex = stoi(input);
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
                cout << "Hero and inventory saved to database!" << endl;
                deleteCurrentGrotte();
                exit(0);
            }
            
        }

        deleteCurrentGrotte(); // Slet grotten hvis hero dør
    }


    Game::~Game(){}