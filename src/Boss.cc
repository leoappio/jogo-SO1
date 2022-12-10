#include "Boss.h"
#include "Point.h"
#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"
#include <iostream>

__BEGIN_API

Boss::Boss(Point p, ALLEGRO_COLOR c, Vector s) : centre(p), color(c), speed(s),
						     enemyLaserSpeed(Vector(-400, 0)),
                       dead(false),
                       dAnim_complete(false), 
                       fire(true),
						     fireSpeed(50),
						     lives(30), dAnim(0)

{
   bossSize = 1;
   targetable = false;
   col = 0;
   row = 0; 
   load_assets();
}

void Boss::load_assets() {
   delayTimer = std::make_shared<Timer> (60);   
   delayTimer->create();
   delayTimer->startTimer();
}

Boss::~Boss() {
   delayTimer.reset();
}

void Boss::hit() {
   lives -= 1;
   if(lives < 1) {
      dead = true;
   }
}

// update Boss movement
void Boss::update(double dt) {
   // update position
   centre = centre + speed * dt;
   
   if(centre.x < 700 && !targetable) {
      targetable = true;
      bossSize = 80;
      speed.x = 0;      
      speed.y = 100;
   }

   if (centre.y > 450 && speed.y > 0) {
      speed.reflectY();
   }
   if (centre.y < 150 && speed.y < 0) {
      speed.reflectY();
   }
   // check fire condition
   if (delayTimer->getCount() > fireSpeed) {
      fire = true;
      delayTimer->srsTimer();
   }else{
      fire = false;
   }
}


__END_API