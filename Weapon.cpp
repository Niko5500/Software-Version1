#include "Weapon.h"

// Weapon.cpp

Weapon::Weapon() {}

Weapon::Weapon(int i, string n, int p, int d, int pr)
{
    id = i;
    name = n;
    power = p;
    durability = d;
    price = pr;
}

Weapon::Weapon(string n, int p, int d, int pr)
{
    id = -1;
    name = n;
    power = p;
    durability = d;
    price = pr;
}

int Weapon::getId()
{
    return id;
}

void Weapon::setId(int newId)
{
    id = newId;
}

string Weapon::getName()
{
    return name;
}

int Weapon::getPower()
{
    return power;
}

int Weapon::getDurability()
{
    return durability;
}

void Weapon::setDurability(int newDurability)
{
    durability = newDurability;
}

int Weapon::getPrice()
{
    return price;
}

void Weapon::print() const {
    cout << name << ":"
         << " Power " << power
         << ", Durability " << durability
         << ", Price " << price << endl;
}

Weapon::~Weapon() {}
