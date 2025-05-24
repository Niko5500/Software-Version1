#include "EnemyFactory.h"

EnemyFactory::EnemyFactory() {}

Enemy* EnemyFactory::createEnemy(string name) // Pointer bruges for polymorphism
{
    if (name == "Horse") 
    {
        int hp = 4 + randomPlusMinus(2);
        int power = 1 + randomPlusMinus(1);
        int xp = 100 + randomPlusMinus(50);

        return new Enemy("Horse", hp, power, xp); 
    } 
    else if (name == "WeakGoblin") 
    {
        int hp = 4 + randomPlusMinus(2);
        int power = 2 + randomPlusMinus(1);
        int xp = 200 + randomPlusMinus(100);
        return new Enemy("Weak Goblin", hp, power, xp);
    }
    else if (name == "StrongGoblin") 
    {
        int hp = 8 + randomPlusMinus(3);
        int power = 3 + randomPlusMinus(1);
        int xp = 400 + randomPlusMinus(100);
        return new Enemy("Strong Goblin", hp, power, xp);
    } 
    else if (name == "StrongerGoblin") 
    {
        int hp = 10 + randomPlusMinus(4);
        int power = 4 + randomPlusMinus(1);
        int xp = 500 + randomPlusMinus(200);
        return new Enemy("Stronger Goblin", hp, power, xp);
    } 
    else if (name == "MightyGoblin") 
    {
        int hp = 15 + randomPlusMinus(5);
        int power = 5 + randomPlusMinus(2);
        int xp = 800 + randomPlusMinus(200);
        return new Enemy("Mighty Goblin", hp, power, xp);
    } 
    else if (name == "KingApe") 
    {
        int hp = 30 + randomPlusMinus(5);
        int power = 5 + randomPlusMinus(2);
        int xp = 1000 + randomPlusMinus(300);
        return new Enemy("KingApe", hp, power, xp);
    } 
    else if (name == "Unicorn") 
    {
        int hp = 50 + randomPlusMinus(10);
        int power = 8 + randomPlusMinus(3);
        int xp = 1500 + randomPlusMinus(500);
        return new Enemy("Unicorn", hp, power, xp);
    } 
    else if (name == "Dragon") 
    {       
        int hp = 200 + randomPlusMinus(10);
        int power = 10 + randomPlusMinus(5);
        int xp = 3000 + randomPlusMinus(1000);
        return new Enemy("Dragon", hp, power, xp);
    }
    else {
        return nullptr;
    }
}

int EnemyFactory::randomPlusMinus(int val) 
{
    return (rand() % 2 == 0 ? -val : val);
}

EnemyFactory::~EnemyFactory() {}