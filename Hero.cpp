#include "Hero.h"

Hero::Hero(){}

Hero::Hero(string n, int h, int p, int l, int x, int g)
{
name = n;
hp = h;
maxHp = h;
power = p;
level = l;
xp = x;
gold = g;
inventory = {};
}

string Hero::getName()
{
	return name;
}

int Hero::getHp()
{
	return hp;
}

void Hero::setHp(int newHp)
{
        hp = newHp;
}

int Hero::getMaxHp()
{
	return maxHp;
}

void Hero::setMaxHp(int newMaxHp)
{
	maxHp = newMaxHp;
}

int Hero::getPower()
{
	return power;
}

void Hero::setPower(int newPower)
{
	power = newPower;
}

int Hero::getLevel()
{
	return level;
}

void Hero::setLevel(int newLevel)
{
	level = newLevel;
}

int Hero::getXp()
{
	return xp;
}

void Hero::setXp(int newXp)
{
	xp = newXp;
}

int Hero::getGold()
{
	return gold;
}

void Hero::setGold(int newGold)
{
	gold = newGold;
}

void Hero::addWeapon(Weapon* weapon)
{
	inventory.push_back(weapon);
}

vector<Weapon*> Hero::getInventory()
{
	return inventory;
}

void Hero::displayInventory()
{
	cout << "Inventory: " << endl;
	for (int i = 0; i < inventory.size(); i++)
	{
		cout << "(" << i << ") " 
		<< inventory[i]->getName() << ":" 
		<< " Power: " << inventory[i]->getPower() << ","
		<< " Durability: " << inventory[i]->getDurability() << ","
		<< " Price: " << inventory[i]->getPrice() << endl;
	}
}

void Hero::equipWeapon(int index)
{
	if (index >= 0 and index < inventory.size())
	{
		equippedWeapon = inventory[index];
		cout << "Equipped: " << equippedWeapon -> getName() << ", "
		<< "Power: " << equippedWeapon ->getPower() << ", "
		<< "Durability: " << equippedWeapon ->getDurability() << endl;
	}
	else 
	{
		cout << "No weapon found" << endl;
	}
}

void Hero::unequipWeapon()
{
	equippedWeapon = nullptr;
}

void Hero::destroyWeapon()
{
    if (equippedWeapon != nullptr)
    {
        if (equippedWeapon->getDurability() <= 0)
        {
            for (auto weapon = inventory.begin(); weapon != inventory.end(); ++weapon)
            {
                if (*weapon == equippedWeapon)
                {
                    inventory.erase(weapon);
                    break;
                }
            }

            cout << equippedWeapon->getName() << " is broken and destroyed" << endl;

            delete equippedWeapon;
            equippedWeapon = nullptr;
        }
    }
}

void Hero::printWeapon()
{
	if (equippedWeapon != nullptr)
	{
		cout << "Weapon: "; equippedWeapon->print();
	}
}

void Hero::decreaseWeaponDurability()
{
	if (equippedWeapon != nullptr)
	{
		equippedWeapon->setDurability(equippedWeapon->getDurability()-1);
	}
}

int Hero::getTotalPower()
{
	if (equippedWeapon != nullptr)
	{
		return power + equippedWeapon->getPower();
	}
	return power;
}

Hero::~Hero()	
{
    for (Weapon* weapon : inventory)
    {
        delete weapon;
    }
    inventory.clear();
}


