#ifndef WINDOW_H
#define WINDOW_H

#include "traits.h"
#include <memory>

__BEGIN_API
class Window{
    public:
     Window(){}
     ~Window(){}
     std::shared_ptr<GameHandler> gameHandler;
     
     void run();

     void draw();
     void drawLives();
     void drawBackground();
     void drawShots();
     void drawEnemySpaceShips();
     void drawBombs();
     void drawBoss();
     void drawSpaceShip();

     void update(double);
     void updateBoss();
     void updateBackground(double dt);
     void updateProjectilePosition(double);
     void updateEnemyPosition(double);
     void updateEnemySpaceShipPosition(double dt);
     void updateBombPosition(double dt);
     void updateBossPosition();
     void chooseBossFrame();
};

__END_API

#endif