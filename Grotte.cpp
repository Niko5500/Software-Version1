#include "Grotte.h"

Grotte::Grotte(string n, int g, vector<Enemy*> e)
{
    name = n;
    gold = g;
    enemies = e;
}

string Grotte::getName()
{
    return name;
}

int Grotte::getGold()
{
    return gold;
}   

vector <Enemy*>& Grotte::getEnemies()
{
    return enemies;
}

void Grotte::removeEnemy(int index)
{
    if (index >= 0 and index < enemies.size())
    {
        delete enemies[index];
        enemies.erase(enemies.begin() + index);
    }
}

bool Grotte::isEmpty() const
{
    return enemies.empty();
}

Grotte::~Grotte()
{
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
}