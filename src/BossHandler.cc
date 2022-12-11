#include "BossHandler.h"
#include "GameHandler.h"
#include "thread.h"

__BEGIN_API

void BossHandler::run(){
    while(!gameHandler->gameOver){        
        if(gameHandler->bossExists && gameHandler->boss == nullptr){
            addBoss(Point(850, 300), al_map_rgb(155, 0, 0), Vector(-50, 0));
        }

        if(gameHandler->boss != nullptr && gameHandler->boss->isToUpdate){
            gameHandler->boss->update(gameHandler->dtToUpdate);
        }

        if(gameHandler->isToBossFire && gameHandler->boss->fire){
            bossFire();
            gameHandler->isToBossFire = false;
        }

        Thread::yield();
    }
}

void BossHandler::addBoss(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   gameHandler->boss = std::make_shared<Boss> (cen, col, spd);
}

void BossHandler::bossFire(){
   Point playerloc;

   if (gameHandler->spaceShip) {
      playerloc = gameHandler->spaceShip->centre;
   }

   Vector aim(0, 0);
   aim.Angle(playerloc, gameHandler->boss->centre+Point(0,50), 0.9);
   gameHandler->addMissile(gameHandler->boss->centre+Point(0,50), al_map_rgb(204, 3, 3), aim, true);
   aim.Angle(playerloc, gameHandler->boss->centre+Point(0,-50), 0.9);
   gameHandler->addMissile(gameHandler->boss->centre+Point(0,-50), al_map_rgb(204, 3, 3), aim, true);
}

__END_API