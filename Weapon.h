#ifndef WEAPON_H
#define WEAPON_H
#include <string>
#include <iostream>
using namespace std;

class Weapon 
{
    protected:
    string name;
    int power;
    int durability;
    int price;

    public:
    Weapon();
    Weapon(string, int, int, int);
    string getName();
    int getPower();
    int getDurability();
    void setDurability(int newDurability);
    int getPrice();
    void print() const;

    ~Weapon();
};

#endif