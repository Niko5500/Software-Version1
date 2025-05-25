#ifndef HERO_H
#define HERO_H
#include "Weapon.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Hero {
	protected:
		int id;
		string name;
		int hp;
		int maxHp;
		int power;
		int level;
		int xp;
		int gold;
		vector <Weapon*> inventory;
    	Weapon* equippedWeapon = nullptr;

	public:
		Hero();
		Hero(int, string, int, int, int, int, int);
		
		int getId();
		string getName();

		int getXp();
		void setXp(int newXp);

		int getLevel();
		void setLevel(int newLevel);

		int getHp();
		void setHp(int newHp);

		int getMaxHp();
		void setMaxHp(int newMaxHp);
				
		int getPower();
		void setPower(int newPower);	

		int getGold();
		void setGold(int newGold);

		vector<Weapon*> getInventory();
		void addWeapon(Weapon* weapon);
		void removeWeapon(Weapon* weapon);
		void displayInventory();
		void printWeapon();
		int getWeaponId();
		void equipWeapon(int index);
		void unequipWeapon();
		void destroyWeapon();
		void decreaseWeaponDurability();
		int getTotalPower();

		~Hero();

};

#endif
