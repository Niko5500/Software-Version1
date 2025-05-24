#include "Armory.h"

Armory::Armory(Hero& h) : hero(h) {}

void Armory::loadWeapons()
{
    // Ryd gamle våben først for at undgå dubletter og memory leaks
    for (Weapon* weapon : weapons) {
        delete weapon;
    }
    weapons.clear();

    WeaponFactory weaponFactory;
    weapons.push_back(weaponFactory.createWeapon("Sword"));
    weapons.push_back(weaponFactory.createWeapon("Axe"));
    weapons.push_back(weaponFactory.createWeapon("Bow"));
    weapons.push_back(weaponFactory.createWeapon("Spear"));
    weapons.push_back(weaponFactory.createWeapon("Doomblade"));
    weapons.push_back(weaponFactory.createWeapon("Shadowfang"));
    weapons.push_back(weaponFactory.createWeapon("Thundercleaver"));
    weapons.push_back(weaponFactory.createWeapon("Nightpiercer"));
    weapons.push_back(weaponFactory.createWeapon("Soulstorm Staff"));
    weapons.push_back(weaponFactory.createWeapon("Titanium Edge"));
    weapons.push_back(weaponFactory.createWeapon("Ebonfang"));
    weapons.push_back(weaponFactory.createWeapon("Dragonscale Shield"));
}

void Armory::displayWeapons()
{   
    cout << "Avaliable Weapons:" << endl;
    for (int i = 0; i < weapons.size();i++)
    {
        cout << "(" << i << ") " << weapons[i]->getName() 
        << " Power: " << weapons[i]->getPower() 
        << " Durability: " << weapons[i]->getDurability() 
        << " Price: " << weapons[i]->getPrice() << endl;

    }
}

string Armory::chooseWeapon(int index)
{
    if (index < 0 or index >= weapons.size())
    {
    return "";
    }
    else 
    {
        return weapons[index]->getName();
    }

}

void Armory::buyWeapon()
{   
    int input; 
    cout << "Buy a weapon by index: ";
    cin >> input;

    string newWeapon = chooseWeapon(input);

    if (newWeapon == "")
    {
        cout << "Invalid choose" << endl;
        return;
    }

    Weapon* selectedWeapon = weapons[input];

    if (hero.getGold() < selectedWeapon->getPrice())
    {   
        cout << "Not enough gold" << endl;
        return;
    }

    else
     {
        WeaponFactory weaponFactory;
        Weapon* weapon = weaponFactory.createWeapon(newWeapon);

        hero.addWeapon(weapon);
        cout << "You have bought: " << newWeapon << endl;
        hero.setGold(hero.getGold()- weapon->getPrice());
    }
}

void Armory::enterArmory()
{
    cout << "You have: " << hero.getGold() << " gold" << endl;
            
    loadWeapons();
}


Armory::~Armory()
{
    for (Weapon* weapon : weapons) {
        delete weapon;
    }
}