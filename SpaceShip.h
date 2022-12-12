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
   int row;
   int col;
   bool dead;
   double dtToUpdate;
   bool isToDraw;
   bool isToUpdate;
   bool isToHit;
   int localLife;
   int hitValue;


   SpaceShip(Point p, ALLEGRO_COLOR c);
   ~SpaceShip();
   void hit();
   void run();
   void update();
   void processInputAction();
   act::action input(ALLEGRO_KEYBOARD_STATE&);
   std::shared_ptr<GameHandler> gameHandler;

  private:
   void shipAnimation();
   void checkBoundary();
};
__END_API
#endif

