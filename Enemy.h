#ifndef Enemy_H
#define Enemy_H
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
                int getPower();
		int getXp();

                ~Enemy();
};

#endif

