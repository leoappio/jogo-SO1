/**
 * @file EnemySpaceShip.cc
 **/

#include "EnemySpaceShip.h"

#include "Point.h"
#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"

const int CREEP_SIZE = 20;

EnemySpaceShip::EnemySpaceShip(Point p, ALLEGRO_COLOR c, Vector s) : centre(p), color(c), speed(s),
   projSpeed(Vector(-400, 0)),
   fireSpeed(rand() % 50 + 30),
   lives(1), dAnim(0),
   dAnim_complete(false), fire(true)
{
   load_assets();
}

void EnemySpaceShip::load_assets() {
   fireDelay = std::make_shared<Timer> (60);   
   fireDelay->create();
   fireDelay->startTimer();
}
	
EnemySpaceShip::~EnemySpaceShip() {
   fireDelay.reset();
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
      // enemy has been hit and killed, proceed through death animation sequence
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
   
   if (centre.y > 600 - CREEP_SIZE &&speed.y > 0)  
      speed.reflectY();
   if (centre.y < 0 - CREEP_SIZE && speed.y < 0)
      speed.reflectY();


   if (fireDelay->getCount() > fireSpeed) {
      fire = true;
      fireDelay->stopTimer();
      fireDelay->resetCount();
      fireDelay->startTimer();
   }
}

void EnemySpaceShip::setFire(bool f) { fire = f; }
ALLEGRO_COLOR EnemySpaceShip::getColor() { return color; }
Vector EnemySpaceShip::getProjSpeed() { return projSpeed; }
int EnemySpaceShip::getSize() { return CREEP_SIZE; }
Point EnemySpaceShip::getCentre() { return centre; }
bool EnemySpaceShip::getDead() { return dead; }   
bool EnemySpaceShip::getFire() { return fire; }
bool EnemySpaceShip::getdAnim_complete() { return dAnim_complete; }
