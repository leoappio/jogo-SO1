#include "EnemySpawn.h"
#include "GameHandler.h"
#include "thread.h"

__BEGIN_API

void EnemySpawn::run(){
    while(!gameHandler->gameOver){
        if(gameHandler->spawnEnemies && !gameHandler->bossExists){
            gameHandler->spawnEnemies = false;
            //spawn enemies in < format
            gameHandler->addEnemySpaceShip(Point(800, 300), al_map_rgb(246, 64, 234),Vector(-180, 0));
            gameHandler->addEnemySpaceShip(Point(900, 350), al_map_rgb(246, 64, 234), Vector(-180, 0));
            gameHandler->addEnemySpaceShip(Point(900, 250), al_map_rgb(246, 64, 234), Vector(-180, 0));
            gameHandler->addEnemySpaceShip(Point(1000, 400), al_map_rgb(246, 64, 234), Vector(-180, 0));
            gameHandler->addEnemySpaceShip(Point(1000, 200), al_map_rgb(246, 64, 234), Vector(-180, 0));
            gameHandler->addEnemySpaceShip(Point(1100, 100), al_map_rgb(246, 64, 234), Vector(-180, 0));
            gameHandler->addEnemySpaceShip(Point(1100, 500), al_map_rgb(246, 64, 234), Vector(-180, 0));

            Point centerPoint(800, 200);
            Vector speedVector(0, 0);

            for (int i = 0; i < 3; i++) {
                speedVector.rollRandom();
                centerPoint.rollRandom();
                gameHandler->addEnemySpaceShip(centerPoint, al_map_rgb(255, 51, 51), speedVector);
	        }


        }
        Thread::yield();
    }
};

__END_API