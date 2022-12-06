/**
 * @file Bomb.cc
 **/

#include "Bomb.h"
#include "Timer.h"


Bomb::Bomb(Point p, ALLEGRO_COLOR c, Vector s) : centre(p), color(c), speed(s), dead(false) {
   lives = 3;
   size = 20;   
   dAnim = 0;
   dAnim_complete = false;
   fire = false;
   
   load_assets();
}

Bomb::~Bomb() {
}

void Bomb::load_assets() {
   row =0; col=0;   
   delayTimer = std::make_shared<Timer> (60);
   delayTimer->create();
   delayTimer->startTimer();
   lives = 3;
   size = 20;   
   dAnim = 0;      
   dAnim_complete = false;
   fire = false; fired=false;
}

void Bomb::hit() {
   lives = lives - 1;
   col++;
   if (lives < 1)
      dead = true;
}
	
void Bomb::draw(std::shared_ptr<Sprite> enemyShip, std::shared_ptr<Sprite> enemyDeath) {
   if (!dead) {
      enemyShip->draw_region(row, col, 40, 41, centre, 0);
   }
   else {
      if (dAnim < 5) deathAnim(enemyDeath);
      else{ dAnim_complete = true;}
   }      
}

void Bomb::deathAnim(std::shared_ptr<Sprite> enemyDeath) { 
   enemyDeath->draw_death_anim(dAnim, centre, 0);
   dAnim++;
}


void Bomb::update(double dt) {
   centre = centre + speed * dt;
				
   if (centre.x < 0)
      dead = true;
   
   if(centre.x<670&&row==0){
      row++;  
   }
   if(centre.x<540&& row==1){
      row++;
   }
   if(centre.x<400 && !fired){
      fire = true;
      fired = true;
      dead = true;
      delayTimer->stopTimer();
      delayTimer->resetCount();
   }
}
