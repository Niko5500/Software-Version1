#include "Game.h"
#include <iostream>
#include <fstream>
using namespace std;

Game::Game(){}

void Game::newHero()
{
    string name;
    int hp = 10;
    int power = 2;
    int level = 1;
    int xp = 0;

    cout << "Enter the name of your Hero: ";
    cin >> name;

    hero = Hero(name, hp, power, level, xp); // Opretter en ny Hero
}

void Game::loadHero(string t)
{
    if (t.find(".txt") == string::npos)
    {
            t += ".txt";
    }

	
    ifstream file(t); // Fil til at gemme Hero data

    if (file.is_open())
    {
        string name;
        int hp, power, level, xp;

        file >> name >> hp >> power >> level >> xp; // Læs Hero data fra fil

        hero = Hero(name, hp, power, level, xp); // Opretter Hero ud fra filen
        displayHero();
    }
    else
    {
        cout << "No saved hero found!" << endl;
    }
}

void Game::saveHero(string t) {
    if (t.find(".txt") == string::npos)
    {
	    t += ".txt";
    }
		
    ofstream file(t);

    if (file.is_open())
    {
        file << hero.getName() << " " << hero.getHp() << " " << hero.getPower() << " " << hero.getLevel() << " " << hero.getXp(); // Gem Hero data til fil
        cout << "Hero saved successfully!" << endl;
    }
    else
    {
        cout << "Failed to save hero!" << endl;
    }
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


Game::~Game(){}























































