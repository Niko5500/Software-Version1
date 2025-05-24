    #include "GrotteFactory.h"

    GrotteFactory::GrotteFactory() {}

    Grotte* GrotteFactory::createGrotte(string name)
    {
        EnemyFactory enemyFactory;

        if (name == "Easy")
        {
            vector<Enemy*> enemiesEasy;
            enemiesEasy.push_back(enemyFactory.createEnemy("Horse"));
            enemiesEasy.push_back(enemyFactory.createEnemy("WeakGoblin"));
            enemiesEasy.push_back(enemyFactory.createEnemy("WeakGoblin"));

            return new Grotte(name, 200, enemiesEasy);
        }
        else if (name == "Medium")
        {
            vector<Enemy*> enemiesMedium;
            enemiesMedium.push_back(enemyFactory.createEnemy("WeakGoblin"));
            enemiesMedium.push_back(enemyFactory.createEnemy("StrongGoblin"));
            enemiesMedium.push_back(enemyFactory.createEnemy("StrongGoblin"));
            enemiesMedium.push_back(enemyFactory.createEnemy("StrongerGoblin"));
            return new Grotte(name, 500, enemiesMedium);
        }
        else if (name == "Hard")
        {
            vector<Enemy*> enemiesHard;
            enemiesHard.push_back(enemyFactory.createEnemy("StrongerGoblin"));
            enemiesHard.push_back(enemyFactory.createEnemy("MightyGoblin"));
            enemiesHard.push_back(enemyFactory.createEnemy("KingApe"));
            enemiesHard.push_back(enemyFactory.createEnemy("StrongerGoblin"));
            return new Grotte(name, 2000, enemiesHard);
        }

        else if (name == "Very Hard")
        {
            vector<Enemy*> enemiesVeryHard;
            enemiesVeryHard.push_back(enemyFactory.createEnemy("Unicorn"));
            enemiesVeryHard.push_back(enemyFactory.createEnemy("KingApe"));
            enemiesVeryHard.push_back(enemyFactory.createEnemy("StrongerGoblin"));
            enemiesVeryHard.push_back(enemyFactory.createEnemy("Unicorn"));
            enemiesVeryHard.push_back(enemyFactory.createEnemy("KingApe"));
            return new Grotte(name, 5000, enemiesVeryHard);

        }

        else if (name == "Extreme")
        {
            vector<Enemy*> enemiesExtreme;
            enemiesExtreme.push_back(enemyFactory.createEnemy("Dragon"));
            return new Grotte(name, 10000, enemiesExtreme);
        }

        else
        {
            cout << "No cave found, try again" << endl;
            return new Grotte("Invalid name",0,{});
        }

    }

    GrotteFactory::~GrotteFactory(){};

