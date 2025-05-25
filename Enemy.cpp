#include "Enemy.h"

Enemy::Enemy(){}

Enemy::Enemy(int i, string n, int h, int p, int x)
{
id = i;
name = n;
hp = h;
power = p;
xp = x;
}

int Enemy::getId()
{
        return id;
}

string Enemy::getName()
{
        return name;
}

int Enemy::getHp()
{
        return hp;
}

int Enemy::getPower()
{
        return power;
}

int Enemy::getXp()
{
	return xp;
}

void Enemy::setHp(int newHp)
{
	hp = newHp;
}

Enemy::~Enemy(){};

