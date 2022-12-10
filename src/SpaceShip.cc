#include "SpaceShip.h"
#include "Point.h"
#include "Vector.h"
#include "Sprite.h"
#include "Action.h"
#include "Laser.h"
#include "Missile.h"
#include "traits.h"
#include "thread.h"
#include "GameHandler.h"
#include "Timer.h"

__BEGIN_API
SpaceShip::SpaceShip(Point p, ALLEGRO_COLOR c) : centre(p), color(c),
                     speed(Vector(0, 0)), row(0), col(0), dead(false), dtToUpdate(0.0), isToDraw(false), isToUpdate(false),
                     isToHit(false), localLife(3), hitValue(0)
{
}

SpaceShip::~SpaceShip() {
   
}

void SpaceShip::run(){
   while(!gameHandler->gameOver){
      hit();
      update();
      processInputAction();
      Thread::yield();
   }
}

void SpaceShip::hit() {
   if(isToHit){
      localLife = localLife - hitValue;

      if(localLife<=0){
         gameHandler->lives = gameHandler->lives - 1;
         gameHandler->enemySpaceShipsList.clear();
         gameHandler->lasersList.clear();
         gameHandler->missileList.clear();
         localLife = 3;
         centre = Point(215, 245);
      }

      if (gameHandler->lives < 1)
         dead = true;
      
      isToHit = false;
   }
}

void SpaceShip::processInputAction(){
  if (gameHandler->lastAction == act::action::MOVE_UP) {
     speed.y -= 250;
  }
  if (gameHandler->lastAction == act::action::MOVE_RIGHT) {
     speed.x += 250;
  }
  if (gameHandler->lastAction == act::action::MOVE_DOWN) {
     speed.y += 250;
  }
  if (gameHandler->lastAction == act::action::MOVE_LEFT) {
     speed.x -= 250;
  }
  if (gameHandler->lastAction == act::action::FIRE_PRIMARY) {
   if(gameHandler->laserShotsTimer->getCount() >= 6){
      gameHandler->addPlayerLaserSingleShot();
   }
  }
  if (gameHandler->lastAction == act::action::FIRE_SECONDARY) {
   if(gameHandler->missileShotsTimer->getCount() >= 20){
      gameHandler->addPlayerMissileSingleShot();
   }
  }
  if (gameHandler->lastAction == act::action::QUIT_GAME) {
     gameHandler->gameOver = true;
  }

  gameHandler->lastAction = act::action::NO_ACTION;
}

void SpaceShip::update() {
   if(isToUpdate){
      centre = centre + speed * dtToUpdate;
      shipAnimation();
      speed = Vector(0, 0);
      checkBoundary();
      isToUpdate = false;
   }
}

void SpaceShip::shipAnimation() {
   if (speed.x > 0) {
      col = 1;
      if (speed.y > 0) row = 2;
      else if (speed.y < 0) row = 0;
      else row = 1;
   } else {
      col = 0;
      if (speed.y > 0) row = 2;
      else if (speed.y < 0) row = 0;
      else row = 1;
   }
}

void SpaceShip::checkBoundary() {   
   if (centre.x > 800 - 16)
      centre.x = 800 - 16;
   else if (centre.x < 16)
      centre.x = 16;   
   if (centre.y > 600 - 16)
      centre.y = 600 - 16;
   else if (centre.y < 16)
      centre.y = 16;
}
__END_API