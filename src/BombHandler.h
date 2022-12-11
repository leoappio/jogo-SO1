#ifndef BOMBHANDLER_H
#define BOMBHANDLER_H

#include "traits.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>
#include "Point.h"
#include "Vector.h"

__BEGIN_API

class BombHandler{
   
  public:
   BombHandler(){};
   ~BombHandler(){};
  
   std::shared_ptr<GameHandler> gameHandler;
   void run();
   void circleLaser(std::shared_ptr<Bomb> bomb);
};

__END_API
#endif