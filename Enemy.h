#ifndef ENEMY_H
#define ENEMY_H
#include <string>
using namespace std;

class Enemy {
        protected:
                string name;
                int hp;
                int power;
                int xp; 
        public:
                Enemy();
                Enemy(string, int, int, int);
                string getName();
                int getHp();
	        void setHp(int newHp);
                int getPower();
		int getXp();

                ~Enemy();
};

#endif

