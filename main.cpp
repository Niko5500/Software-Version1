#include "Hero.h"
#include "Enemy.h"
#include "Game.h"
#include "Fight.h"
#include <iostream>
using namespace std;

int main(){

   Game game;
   Enemy dummyEnemy("Dummy",1,1,1);
   Fight fight(game.getHero(), dummyEnemy);


   int valg;
   cout << "(0) new Game (1) Load Game: " << endl;
   cin >> valg;    cin.ignore();
   game.valgHero(valg);

   bool heroLever = true;
   while (heroLever)
   {
    game.gameMenu(fight);

    fight.printHero();
    fight.printEnemy();
    cin.ignore();

    // Kamp
    	while (fight.getHero().getHp() > 0 && fight.getEnemy().getHp() > 0)
   	 {
        	cout <<  "\nTryk enter for at fighte";
        	cin.get();

        	fight.getHeroFight(); 
       		fight.getEnemyFight();

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

		fight.getHero().setHp(fight.getHero().getMaxHp());
		fight.setHeroXp();
		fight.setHeroLevel();	
		
		heroLever = true;
    	 }

   }
   
   return 0;
}

