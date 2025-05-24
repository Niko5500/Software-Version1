#include "Fight.h"
#include <iostream>

using namespace std;

Fight::Fight(Hero& h, Enemy& e, Grotte& g) : hero(h), enemy(e), grotte(g) {}

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
    cout << "Gold: " << hero.getGold() << endl;
}

Hero& Fight::setHeroXp()
{
    int xpGained = enemy.getXp();
    hero.setXp(hero.getXp() + xpGained);
    return hero;
}

Hero& Fight::setHeroGold()
{
    int goldGained = grotte.getGold();
    hero.setGold(hero.getGold() + goldGained);
    return hero;
}

Hero& Fight::updateHero()
{
    if (hero.getXp() >= 1000)
    {
        hero.setLevel(hero.getLevel() + 1);
        hero.setHp(hero.getHp() + 2);
        hero.setMaxHp(hero.getMaxHp() + 2);
        hero.setPower(hero.getPower() + 1);
        hero.setXp(hero.getXp() - 1000);

        cout << "New level: " << hero.getLevel() << endl;
    }
    return hero;
}

Enemy& Fight::updateEnemyFight()
{
    enemy.setHp(enemy.getHp() - hero.getTotalPower());
    return enemy;
}

Enemy& Fight::getEnemy()
{
    return enemy;
}

void Fight::setEnemy(const Enemy& newEnemy)
{
    enemy = newEnemy;
}

Grotte& Fight::getGrotte()
{
    return grotte;
}

void Fight::setGrotte(const Grotte& newGrotte)
{
    grotte = newGrotte;
}

void Fight::printEnemy()
{
    cout << "" << endl;
    cout << "Navn: " << enemy.getName() << endl;
    cout << "HP: " << enemy.getHp() << endl;
    cout << "Power: " << enemy.getPower() << endl;
    cout << "XP: " << enemy.getXp() << endl;
    cout << "" << endl;
}

bool Fight::runFight()
{
    while (getHero().getHp() > 0 && getEnemy().getHp() > 0)
    {
        cout << "--------------------------" << endl;
        cout << "| Tryk enter for at fighte |" << endl;
        cout << "--------------------------";
        cin.get();

        updateHeroFight();
        updateEnemyFight();

        printHero();
        printEnemy();
    }

    if (getHero().getHp() <= 0)
    {
        cout << "You lost" << endl;
        return false;
    }
    else if (getEnemy().getHp() <= 0)
    {
        cout << "-----------------------------------" << endl;
        cout << getEnemy().getName() << " defeated!" << endl;
        cout << "You won!" << endl;
        
        getHero().setHp(getHero().getMaxHp());
        setHeroXp();
        updateHero();

        cout << "-----------------------------------" << endl;
        cout << "Hero Stats:" << endl;
        printHero();
        getHero().decreaseWeaponDurability();
        cout << "" << endl;
        getHero().printWeapon();
        getHero().destroyWeapon();
        cout << "" << endl;
        cout << "-----------------------------------" << endl;

        return true;
    }

    return false;
}

Fight::~Fight() {}




