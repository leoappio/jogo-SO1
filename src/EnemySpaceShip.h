/**
 * @file EnemySpaceShip.h
 **/

#ifndef CREEP_H
#define CREEP_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <memory>

#include "EnemySpaceShip.h"
#include "Point.h"
#include "Vector.h"


struct Point;
struct Vector;
class Timer;
class Sprite;

class EnemySpaceShip{
   
  private:
   std::shared_ptr<Timer> delayTimer;
	
  public:
   EnemySpaceShip(Point p, ALLEGRO_COLOR c, Vector s);
   ~EnemySpaceShip();
   Point centre;
   ALLEGRO_COLOR color;
   Vector speed;
   bool dead;
   int size = 20;
   Vector enemyLaserSpeed;
   int fireSpeed;
   int lives;
   int dAnim;
   bool dAnim_complete;
   bool fire;
   
   void update(double dt);
   void load_assets();
   void deathAnim(std::shared_ptr<Sprite>);
   void hit();
   void draw(std::shared_ptr<Sprite> ship, std::shared_ptr<Sprite> death);  
};
#endif
