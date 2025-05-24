#include "Weapon.h"

Weapon::Weapon() {}

Weapon::Weapon(string n, int p, int d, int pr)
{
    name = n;
    power = p;
    durability = d;
    price = pr;
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
         << ", Holdbarhed " << durability
         << ", Pris " << price << endl;
}


Weapon::~Weapon() {}