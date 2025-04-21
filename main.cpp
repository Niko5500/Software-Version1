#include "Hero.h"
#include "Enemy.h"
#include "Game.h"
#include "Fight.h"
#include <iostream>
using namespace std;

int main(){

    Game game;

    int valg;
    cout << "(0) new Game (1) Load Game: " << endl;
    cin >> valg;
    cin.ignore();
    game.valgHero(valg);
    game.displayHero();

    Enemy dummyEnemy("Dummy",1,1,1);
    Fight fight(game.getHero(), dummyEnemy);

   bool heroLever = true;
   while (heroLever)
   {
    game.gameMenu(fight);

    // Vis stats før kamp
    fight.printHero();
    fight.printEnemy();
    cin.ignore();
    // Kampsløjfe
    while (fight.getHero().getHp() > 0 && fight.getEnemy().getHp() > 0)
    {
        cout << "\nTryk enter for at fighte";
        cin.get(); // ← her venter vi på brugeren

        fight.getHeroFight();  // Enemy angriber Hero
        fight.getEnemyFight(); // Hero angriber Enemy

        fight.printHero();
        fight.printEnemy();
    }

    // Resultat
    if (fight.getHero().getHp() <= 0)
    {
        cout << "You lost" << endl;
	heroLever = false;
    }
    else if (fight.getEnemy().getHp() <= 0)
    {
	cout << fight.getEnemy().getName() << " deafeated!"<< endl;
        cout << "You won!" << endl;	
	heroLever = true;
    }
   }
    return 0;
}

