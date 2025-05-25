#include"WeaponFactory.h"

WeaponFactory::WeaponFactory() {}

Weapon* WeaponFactory::createWeapon(string name)
{
    if (name == "Sword")
    {
        return new Weapon("Sword", 1, 20, 500);
    }
    else if (name == "Axe")
    {
        return new Weapon("Axe", 1, 40, 700);
    }
    else if (name == "Bow")
    {
        return new Weapon("Bow", 2, 30, 600);
    }
    else if (name == "Spear")
    {
        return new Weapon("Spear", 3, 15, 650);
    }

    // Stronger, legendary weapons
    else if (name == "Doomblade")
    {
        return new Weapon("Doomblade", 5, 50, 10000);  
    }
    else if (name == "Shadowfang")
    {
        return new Weapon("Shadowfang", 6, 45, 20000);  
    }
    else if (name == "Thundercleaver")
    {
        return new Weapon("Thundercleaver", 10, 30, 30000); 
    }
    else if (name == "Nightpiercer")
    {
        return new Weapon("Nightpiercer", 8, 1000, 50000);  
    }
    else if (name == "Soulstorm Staff")
    {
        return new Weapon("Soulstorm Staff",10 ,1000, 75000);  
    }
    else if (name == "Titanium Edge")
    {
        return new Weapon("Titanium Edge", 15, 1000, 90000); 
    }
    else if (name == "Ebonfang")
    {
        return new Weapon("Ebonfang", 20, 500, 100000); 
    }
    else if (name == "Dragonscale Shield")
    {
        return new Weapon("Dragonscale Shield", 20, 100000, 200000);  
    }

    else if (name == "Star destroyer")
    {
        return new Weapon("Star destroyer", 50, 1000000, 500000);  
    }
    
    else
    {
        return nullptr;
    }
}

WeaponFactory::~WeaponFactory() {}