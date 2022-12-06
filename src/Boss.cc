/**
 * @file Boss.cc
 **/

#include "Boss.h"

#include "Point.h"
#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"
#include <iostream>


const int BOSS_HP = 50;

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

void Boss::draw(std::shared_ptr<Sprite> bossShip, std::shared_ptr<Sprite> bossDeath) {
   chooseFrame();
   if (!dead) {
      bossShip -> draw_boss(row, col, 200, 200, centre, 0);
   }
   else {
      if (dAnim < 5) 
	 deathAnim(bossDeath);
      else
	 dAnim_complete = true;
   }
}

//this function is going to be a little interesting
void Boss::deathAnim(std::shared_ptr<Sprite> bossDeath) {
   bossDeath->draw_death_anim(dAnim, centre, 0);
   dAnim++;
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

void Boss::chooseFrame() {
   if (lives > BOSS_HP){
      spriteSheetIndex = 0;
   }

   if (lives <= BOSS_HP && spriteSheetIndex < 3) { 
      fireSpeed = rand()%50+20;
      speed = speed * 1.1;
      bossSize = 70;
      spriteSheetIndex++;
   }

   if (lives <= 20 && spriteSheetIndex < 8) {
      fireSpeed = rand()%30+20;
      speed = speed * 1.1;
      bossSize = 60;
      spriteSheetIndex++;
   }

   row = spriteSheetIndex / 3;
   col = spriteSheetIndex % 3;

}
