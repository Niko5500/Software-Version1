#ifndef GROTTE_H
#define GROTTE_H

#include "Enemy.h"
#include "EnemyFactory.h"
#include <string>
#include <vector>
using namespace std;

class Grotte
{
    public:
        Grotte(string, int, vector<Enemy*>);
        string getName();
        int getGold();
        vector <Enemy*>& getEnemies();
        void removeEnemy(int index);
        bool isEmpty() const;

        ~Grotte();
    protected:
        string name;
        int gold;
        vector<Enemy*> enemies; 
};

#endif