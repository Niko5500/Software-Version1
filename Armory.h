#ifndef ARMORY_H
#define ARMORY_H

#include "Weapon.h"
#include "Hero.h"
#include "WeaponFactory.h"
#include "Game.h"
#include <vector>
#include <iostream>

class Armory
{   
    protected:
        vector <Weapon*> weapons;
        WeaponFactory weaponFactory;
        Hero& hero;
    public:
        Armory();
        Armory(Hero& hero);
        void loadWeapons();
        void displayWeapons();
        string chooseWeapon(int index);
        void buyWeapon();
        void enterArmory();

        ~Armory();
};


#endif