#ifndef ENEMY_H
#define ENEMY_H
#include <string>
using namespace std;

class Enemy {
        protected:
                int id;
                string name;
                int hp;
                int power;
                int xp; 
        public:
                Enemy();
                Enemy(int, string, int, int, int);
                int getId();
                string getName();
                int getHp();
	        void setHp(int newHp);
                int getPower();
		int getXp();

                ~Enemy();
};

#endif

