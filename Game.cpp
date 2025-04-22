#include "Game.h"
#include <iostream>
#include <fstream>
using namespace std;

Game::Game(){}

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

    cout << "Enter the name of your Hero: ";
    cin >> name;

    hero = Hero(name, hp, power, level, xp);
}

void Game::loadHero(string t)
{
    if (t.find(".txt") == string::npos)
    {
            t += ".txt";
    }

	
    ifstream file(t); // Åbne fil med Hero

    if (file.is_open())
    {
        string name;
        int hp, power, level, xp;

        file >> name >> hp >> power >> level >> xp;

        hero = Hero(name, hp, power, level, xp);
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
        cout << "Kunne ikke åbne filen til skrivning!" << endl;
        return;
    }

    file << hero.getName() << " "
         << hero.getHp() << " "
         << hero.getPower() << " "
         << hero.getLevel() << " "
         << hero.getXp();

    cout << "Hero saved successfully!" << endl;
}


void Game::displayHero()
{
    cout << "Hero Name: " << hero.getName() << endl;
    cout << "HP: " << hero.getHp() << endl;
    cout << "Power: " << hero.getPower() << endl;
    cout << "Level: " << hero.getLevel() << endl;
    cout << "XP: " << hero.getXp() << endl;
}

void Game::valgHero(int n)
{
    while (n != 0 && n != 1)
    {
        cout << "Ugyldigt valg. Indtast 0 for ny helt eller 1 for at loade en helt: ";
        cin >> n;
    }

    if (n == 0)
    {
        newHero();
    }
    else
    {
        string filnavn;
        cout << "Indtast filnavn på helten du vil loade: ";
        cin >> filnavn;
        loadHero(filnavn);
    }
}

void Game::gameMenu(Fight& fight)
{
	cout << "Your options are (0) Fight Monsters (4) save and exit:";
	
	int input;
	cin >> input;
	cout << "Your choice: " << input << endl;

	if (input == 0)
	{
		Enemy& chooseEnemy = fight.chooseEnemy();
	}

	else if (input == 4)
	{
		string fileName;
		cout << "Indtask et filnavn til at gemme dit spil: ";
		cin >> fileName;

		saveHero(fileName);
		exit(0);		
	}

	else
	{
		cout << "Ugyldigt valg";
		gameMenu(fight);
	}
}

Game::~Game(){}























































