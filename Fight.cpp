#include "Fight.h"
#include <iostream>

using namespace std;

Fight::Fight(Hero& h, Enemy& e) : hero(h), enemy(e)
{
    enemies = {
        Enemy("Horse", 4, 1, 100),
        Enemy("Weak Goblin", 4, 2, 200),
        Enemy("Strong Goblin", 8, 3, 400),
        Enemy("Stronger Goblin", 10, 4, 500),
        Enemy("Mighty Goblin", 15, 5, 800),
        Enemy("KingApe", 30, 5, 1000),
        Enemy("Unicorn", 5, 8, 1500),
        Enemy("Dragon", 100, 10, 3000)
    };
}

Hero& Fight::updateHeroFight()
{
    hero.setHp(hero.getHp() - enemy.getPower());

    return hero;
}

Hero& Fight::getHero()
{
    return hero;
}

void Fight::printHero()
{
    cout << "" << endl;
    cout << "Navn: " << hero.getName() << endl;
    cout << "HP: " << hero.getHp() << endl;
    cout << "Power: " << hero.getPower() << endl;
    cout << "Level: " << hero.getLevel() << endl;
    cout << "XP: " << hero.getXp() << endl;
    cout << "" << endl;
}

Hero& Fight::setHeroXp()
{
    int xpGained = enemy.getXp();
    hero.setXp(hero.getXp() + xpGained);

    cout << "New Hero XP: " << hero.getXp() << endl;

    return hero;
}

Hero& Fight::updateHero()
{
    if (hero.getXp() >= 1000)
    {
        hero.setLevel(hero.getLevel() + 1);
	hero.setHp(hero.getHp()+2);
	hero.setMaxHp(hero.getMaxHp()+2);
	hero.setPower(hero.getPower()+1);	
        hero.setXp(hero.getXp() - 1000);

        cout << "New level: " << hero.getLevel() << endl;
    }
    return hero;
}

Enemy& Fight::updateEnemyFight()
{
    enemy.setHp(enemy.getHp() - hero.getPower());

    return enemy;
}

Enemy& Fight::getEnemy()
{
    return enemy;
}

void Fight::printEnemy()
{
    cout << "Navn: " << enemy.getName() << endl;
    cout << "HP: " << enemy.getHp() << endl;
    cout << "Power: " << enemy.getPower() << endl;
    cout << "XP: " << enemy.getXp() << endl;
    cout << "" << endl;
}


Enemy& Fight::chooseEnemy()
{	
    for (int i = 0; i < enemies.size(); i++)
    {
        cout << i << " : " << enemies[i].getName() << endl;
    }

    
    int choice;
    cout << "Vælg en fjende med indeks fra 0-7: ";
    cin >> choice;
 
    while (choice < 0 and choice >= enemies.size())
    {
        cout << "Ugyldigt valg, prøv igen (0-7): ";
        cin >> choice;
    }
	
    enemy = enemies[choice];
    return enemy;  
}


Fight::~Fight(){};



