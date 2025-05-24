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

void Game::loadHero(string t)
{
    if (t.find(".txt") == string::npos)
    {
        t += ".txt";
    }

    ifstream file(t);

    if (file.is_open())
    {
        string name;
        int hp, power, level, xp, gold;

        file >> name >> hp >> power >> level >> xp >> gold;
        hero = Hero(name, hp, power, level, xp, gold);

        // Læs inventory
        int invSize;
        file >> invSize;

        for (int i = 0; i < invSize; ++i)
        {
            string wName;
            int wPower, wDurability, wPrice;

            file >> wName >> wPower >> wDurability >> wPrice;

            Weapon* w = new Weapon(wName, wPower, wDurability, wPrice);
            hero.addWeapon(w);
        }

        displayHero();
    }
    else
    {
        cout << "No saved hero found!" << endl;
    }
}


void Game::saveHero(string t)
{
    if (t.find(".txt") == string::npos)
    {
        t += ".txt";
    }

    ofstream file(t);

    if (!file)
    {
        cout << "Could not open file and write to it!" << endl;
        return;
    }

    file << hero.getName() << " "
         << hero.getHp() << " "
         << hero.getPower() << " "
         << hero.getLevel() << " "
         << hero.getXp() << " "
         << hero.getGold() << endl;

         vector<Weapon*> inv = hero.getInventory();
    file << inv.size() << endl;
    for (Weapon* w : inv)
    {
        file << w->getName() << " ";
        file << w->getPower() << " ";
        file << w->getDurability() << " ";
        file << w->getPrice() << " " << endl;
    }

    cout << "Hero saved successfully!" << endl;
}


void Game::displayHero()
{
    cout << "Hero Name: " << hero.getName() << endl;
    cout << "HP: " << hero.getHp() << endl;
    cout << "Power: " << hero.getPower() << endl;
    cout << "Level: " << hero.getLevel() << endl;
    cout << "XP: " << hero.getXp() << endl;
    cout << "Gold: " << hero.getGold() << endl;
}

// Tjekker filer i niværende mappe er txt filer og udskriver
void Game::listTxtFiles()
{
    cout << "Saved files: " << endl;
    for (const auto& file : filesystem::directory_iterator("."))
    {
        if (file.path().extension() == ".txt" and file.path().filename() != "CMakeCache.txt") 
        {
            cout << "- " << file.path().stem() << endl; 
        }
    }
    cout << "" << endl;
}

void Game::valgHero(int n)
{
    while (n != 0 && n != 1)
    {
        cout << "Incorret. Type 0 for new hero or 1 to load an existing hero: ";
        cin >> n;
    }

    if (n == 0)
    {
        newHero();
    }
    else
    {
        listTxtFiles();
        string filnavn;
        cout << "Type file name for hero: ";
        cin >> filnavn;
        cout << "-----------------------------------" << endl;
        cout << "Hero stats: " << endl;
        cout << "-----------------------------------" << endl;
        loadHero(filnavn);
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


void Game::start()
 {
    srand(time(0)); // bruges til tilfælde tal. det er en timer der starter når programmet startes.
    Enemy dummyEnemy("Dummy", 1, 1, 1);
    Grotte dummyGrotte("Dummy", 1, {});
    Fight fight(hero, dummyEnemy, dummyGrotte);
    Armory armory(hero);

    int valg;
    cout << "(0) new Game (1) Load Game: ";
    cin >> valg;    
    cin.ignore();
    cout << "" << endl;
    valgHero(valg);

    cout << "" << endl;
    gameRules();
    cout << "" << endl;


    bool heroLever = true;
    while (heroLever) 
    {   
        cout << "Your options are: (0) Fight Monsters, (2) Go to Armory, (4) save and exit: ";
        int input;
        cin >> input;
        cout << "" << endl;

        if (input == 0) // Vælg grotte og besejr ALLE fjender i den
        {
            currentGrotte = chooseGrotte();
            fight.setGrotte(*currentGrotte);
            cout << "" << endl;
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

                // Kamp
                fight.runFight();

                // Fjern besejret enemy
                currentGrotte->removeEnemy(currentEnemyIndex);
            }
            cout << "" << endl;
            cout << "You have cleared Grotten" << endl;
            fight.setHeroGold();
            cout << "Hero gained gold: " << currentGrotte->getGold() << endl;
            deleteCurrentGrotte(); // Grotten er tom, så den slettes
        }

        if (input == 2) // gå til Armory
        {
        cout << "<><><><><><><><><><><><><><><><><><><><><>" << endl;
        cout << "Welcome to the Armory" << endl;
        cout << "You can buy weapons here" << endl;
        cout << "" << endl;
        bool inArmory = true;
        armory.loadWeapons();
        armory.displayWeapons();
            while (inArmory)
            {
            cout << "--------------------------------------------" << endl;
            armory.enterArmory();
            cout << "" << endl;
            cout << "Your option are: (0) Buy Weapons, (1) Equip weapon, (2) Unequip weapon, (3) Exit Armory: ";
            char choose;
            cin >> choose;
            if (choose == '0')
            {
                armory.buyWeapon();
                cout << "" << endl;
            }

            if (choose == '1')
            {   cout << "" << endl;
                hero.displayInventory();
                cout << "" << endl;
                cout << "Choose a weapon to equip: ";
                int input;
                cin >> input;
                cout << "" << endl;
                hero.equipWeapon(input);
            }

            else if (choose == '2')
            {
                cout << "Unequiped "; hero.printWeapon();
                cout << "" << endl;
                hero.unequipWeapon();
            }

            else if (choose == '3')
            {   
                cout << "Exiting armory" << endl;
                inArmory = false;
            }
        }
        }

        else if (input == 4) // Exit
        {
            string fileName;
            cout << "Write a filename to save game: ";
            cin >> fileName;

            saveHero(fileName);
            deleteCurrentGrotte();
            exit(0);	
        }
    }

    deleteCurrentGrotte(); // Slet grotten, hvis hero dør
}

Game::~Game(){}