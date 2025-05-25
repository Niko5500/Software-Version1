#ifndef WEAPON_H
#define WEAPON_H
#include <string>
#include <iostream>
using namespace std;

class Weapon 
{
protected:
    int id;
    string name;
    int power;
    int durability;
    int price;

public:
    Weapon();
    Weapon(int i, string n, int p, int d, int pr);  // med id
    Weapon(string n, int p, int d, int pr);         // uden id
    int getId();
    void setId(int newId);
    string getName();
    int getPower();
    int getDurability();
    void setDurability(int newDurability);
    int getPrice();
    void print() const;
    ~Weapon();
};


#endif