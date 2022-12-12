#ifndef BOSSHANDLER_H
#define BOSSHANDLER_H

#include "traits.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>
#include "Point.h"
#include "Vector.h"

__BEGIN_API

class BossHandler{
   
  public:
   BossHandler(){};
   ~BossHandler(){};
  
   std::shared_ptr<GameHandler> gameHandler;
   void run();
   void addBoss(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd);
   void bossFire();
};

__END_API
#endif
