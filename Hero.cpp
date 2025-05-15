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

Hero::~Hero(){};


