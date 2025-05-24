#ifndef WEAPONFACTORY_H
#define WEAPONFACTORY_H

#include "Weapon.h"
#include <string>
using namespace std;

class WeaponFactory
{
    public:
    WeaponFactory();
    Weapon* createWeapon(string name);
    ~WeaponFactory();
};

#endif