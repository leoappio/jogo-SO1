#include "BombHandler.h"
#include "GameHandler.h"
#include "thread.h"

__BEGIN_API

void BombHandler::run(){
    while(!gameHandler->gameOver){
        if(gameHandler->generateBomb){
            Point pt(800, 200);
            pt.y=pt.y+(rand()%200);
            gameHandler->addBomb(pt, al_map_rgb(204,3,3), Vector(-60, 0));
            gameHandler->generateBomb = false;
        }

        for (std::list< std::shared_ptr<Bomb> >::iterator it_bomb = 
            gameHandler->bombEnemiesList.begin(); it_bomb != gameHandler->bombEnemiesList.end(); ++it_bomb) {
            if((*it_bomb)->circleLaserShot){
                circleLaser((*it_bomb));
                (*it_bomb)->circleLaserShot = false;
            }

            if((*it_bomb)->isToUpdate){
                (*it_bomb)->update(gameHandler->dtToUpdate);
                (*it_bomb)->isToUpdate = false;
            }
        }

        Thread::yield();
    }
}

void BombHandler::circleLaser(std::shared_ptr<Bomb> bomb) {
   for(int i = -500; i <= 500; i += 325) {
      for(int j = -500; j <= 500; j += 325) {
	      gameHandler->addLaser(bomb->centre, bomb->color, Vector(i, j));
      }
   }
   bomb->fire = false;
}

__END_API
