#ifndef BOMB_H
#define BOMB_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <memory>
#include <iostream>
#include <stdexcept>

#include "Point.h"
#include "Vector.h"
#include "Sprite.h"
#include "threads.h"
__BEGIN_API

struct Point;
struct Vector;
class Timer;
class Bomb{
   
  private:
   std::shared_ptr<Timer> delayTimer;
	
  public:
   Bomb(Point cen, ALLEGRO_COLOR col, Vector spd);
   ~Bomb();
   Point centre;
   ALLEGRO_COLOR color;
   Vector speed;
   bool dead;
   int size;
   int row, col;
   int lives;
   int dAnim;
   bool dAnim_complete;
   bool fire, fired;
   bool isToUpdate;
   bool circleLaserShot;
  
   void update(double dt);
   void load_assets();
   void hit();
};

__END_API
#endif
