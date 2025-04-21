#include "Hero.h"

Hero::Hero(){}

Hero::Hero(string n, int h, int p, int l, int x)
{
name = n;
hp = h;
power = p;
level = l;
xp = x;
}

string Hero::getName()
{
	return name;
}

int Hero::getHp()
{
	return hp;
}

int Hero::getPower()
{
	return power;
}

int Hero::getLevel()
{
	return level;
}

int Hero::getXp()
{
	return xp;
}

void Hero::takeDamage(int damage)
{
	hp -= damage;
}

void Hero::setHp(int newHp)
{
	hp = newHp;
}


Hero::~Hero(){};


