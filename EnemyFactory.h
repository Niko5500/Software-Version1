#ifndef ENEMYFACTORY_H
#define ENEMYFACTORY_H

#include "Enemy.h"
#include <vector>
#include <cstdlib>

class EnemyFactory
{
    public:
        EnemyFactory();
        Enemy* createEnemy(string name);
        int randomPlusMinus(int val);
        ~EnemyFactory();
    private:
};

#endif

