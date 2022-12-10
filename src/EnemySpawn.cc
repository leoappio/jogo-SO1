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
        }
        Thread::yield();
    }
};

__END_API