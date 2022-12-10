#ifndef COLLISIONANDLIFE_H
#define COLLISIONANDLIFE_H

#include <iostream>
#include <memory>
#include "traits.h"
#include "thread.h"
#include "GameHandler.h"

__BEGIN_API
class CollisionAndLife{
    public:
     std::shared_ptr<GameHandler> gameHandler;
     CollisionAndLife(){}
     ~CollisionAndLife(){}
     void run();
     void collision();
     void checkCollisionOnPlayer();
     void checkCollisionOnEnemies();
     void checkCollidingEnemyWithPlayer();
     void clean();
     void cullPlayer();
     void cleanLasers();
     void cleanMissiles();
     void cullEnemies();
     bool doHitboxesIntersect(const Point&, const int&, const Point&, const int&);   
     bool doColorsMatch(const ALLEGRO_COLOR&, const ALLEGRO_COLOR&);
     bool isPointBoxCollision(const Point&, const Point&, const int&);
   
};

__END_API
#endif