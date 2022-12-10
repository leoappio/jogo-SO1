/**
 * @file SpaceShip.h
 */

#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Action.h"
#include "Point.h"
#include "Vector.h"
#include "traits.h"
#include <memory>

__BEGIN_API
struct Point;
struct Vector;
class Sprite;


class SpaceShip {
  public:
   Point centre;  
   ALLEGRO_COLOR color; 
   
   Vector speed;
   float lives; 
   int row;
   int col;
   bool dead;


   SpaceShip(Point p, ALLEGRO_COLOR c);
   ~SpaceShip();
   void hit(int);
   void draw(std::shared_ptr<Sprite> spaceShipSprite, int flags);
   void update(double);
   act::action input(ALLEGRO_KEYBOARD_STATE&);

  private:
   void shipAnimation();
   void checkBoundary();
};
__END_API
#endif

