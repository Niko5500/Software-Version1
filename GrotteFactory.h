#ifndef GrotteFactory_H
#define GrotteFactory_H

#include "Grotte.h"
#include "EnemyFactory.h"
#include <iostream>

class GrotteFactory
{
public:
GrotteFactory();
Grotte* createGrotte(string name);
~GrotteFactory();

};

#endif