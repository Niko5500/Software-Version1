#include "EnemyFactory.h"

EnemyFactory::EnemyFactory() {}

Enemy* EnemyFactory::createEnemy(string name) // Pointer bruges for polymorphism
{
    if (name == "Horse") 
    {   int id = 1;
        int hp = 4 + randomPlusMinus(2);
        int power = 1 + randomPlusMinus(1);
        int xp = 100 + randomPlusMinus(50);

        return new Enemy(id, "Horse", hp, power, xp); 
    } 
    else if (name == "WeakGoblin") 
    {   
        int id = 2;
        int hp = 4 + randomPlusMinus(2);
        int power = 2 + randomPlusMinus(1);
        int xp = 200 + randomPlusMinus(100);
        return new Enemy(id, "Weak Goblin", hp, power, xp);
    }
    else if (name == "StrongGoblin") 
    {   int id = 3;
        int hp = 8 + randomPlusMinus(3);
        int power = 3 + randomPlusMinus(1);
        int xp = 400 + randomPlusMinus(100);
        return new Enemy(id, "Strong Goblin", hp, power, xp);
    } 
    else if (name == "StrongerGoblin") 
    {
        int id = 4;
        int hp = 10 + randomPlusMinus(4);
        int power = 4 + randomPlusMinus(1);
        int xp = 500 + randomPlusMinus(200);
        return new Enemy(id, "Stronger Goblin", hp, power, xp);
    } 
    else if (name == "MightyGoblin") 
    {
        int id = 5;
        int hp = 15 + randomPlusMinus(5);
        int power = 5 + randomPlusMinus(2);
        int xp = 800 + randomPlusMinus(200);
        return new Enemy(id, "Mighty Goblin", hp, power, xp);
    } 
    else if (name == "KingApe") 
    {
        int id = 6;
        int hp = 30 + randomPlusMinus(5);
        int power = 5 + randomPlusMinus(2);
        int xp = 1000 + randomPlusMinus(300);
        return new Enemy(id, "KingApe", hp, power, xp);
    } 
    else if (name == "Unicorn") 
    {
        int id = 7;
        int hp = 50 + randomPlusMinus(10);
        int power = 8 + randomPlusMinus(3);
        int xp = 1500 + randomPlusMinus(500);
        return new Enemy(id, "Unicorn", hp, power, xp);
    } 
    else if (name == "Dragon") 
    {    
        int id = 8;   
        int hp = 200 + randomPlusMinus(10);
        int power = 10 + randomPlusMinus(5);
        int xp = 3000 + randomPlusMinus(1000);
        return new Enemy(id ,"Dragon", hp, power, xp);
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