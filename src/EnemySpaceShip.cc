/**
 * @file EnemySpaceShip.cc
 **/

#include "EnemySpaceShip.h"

#include "Point.h"
#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"
#include <iostream>

EnemySpaceShip::EnemySpaceShip(Point p, ALLEGRO_COLOR c, Vector s) : centre(p), color(c), speed(s), dead(false),
   enemyLaserSpeed(Vector(-400, 0)),
   fireSpeed(rand() % 50 + 30),
   lives(1), dAnim(0),
   dAnim_complete(false), fire(true)
{
   load_assets();
}

void EnemySpaceShip::load_assets() {
   delayTimer = std::make_shared<Timer> (60);   
   delayTimer->create();
   delayTimer->startTimer();
}
	
EnemySpaceShip::~EnemySpaceShip() {
   delayTimer.reset();
}

void EnemySpaceShip::hit() {
   lives = lives - 1;
   if (lives < 1)
      dead = true;
}
	
void EnemySpaceShip::draw(std::shared_ptr<Sprite> enemyShip, std::shared_ptr<Sprite> enemyDeath) {
   if (!dead) {
      enemyShip->draw_tinted(centre, color, 0);
   }
   else {
      if (dAnim < 5) deathAnim(enemyDeath);
      else dAnim_complete = true;
   }
}

void EnemySpaceShip::deathAnim(std::shared_ptr<Sprite> enemyDeath) {
   enemyDeath->draw_death_anim(dAnim, centre, 0);
   dAnim++;
}

void EnemySpaceShip::update(double dt) {
   centre = centre + speed * dt;
				
   if (centre.x < 0) {
      dead = true;
      return;
   }
   
   if (centre.y > 600 - 20 &&speed.y > 0)  
      speed.reflectY();
   if (centre.y < 0 - 20 && speed.y < 0)
      speed.reflectY();


   if (delayTimer->getCount() > fireSpeed) {
      fire = true;
      delayTimer->stopTimer();
      delayTimer->resetCount();
      delayTimer->startTimer();
   }
}
