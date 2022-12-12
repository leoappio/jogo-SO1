#include "Window.h"
#include "GameHandler.h"
#include "Timer.h"
#include "EnemySpaceShip.h"
#include "thread.h"

__BEGIN_API

void Window::run(){
    while(!gameHandler->gameOver){
        if(gameHandler->isToUpdate){
            update(gameHandler->dtToUpdate);
        }

        if(gameHandler->isToDraw){
            draw();
        }
        Thread::yield();
    }
}

void Window::update(double dt) {
   updateBackground(dt);
   if (gameHandler->spaceShip) {
      gameHandler->spaceShip->isToUpdate = true;
      gameHandler->spaceShip->dtToUpdate = dt;
   }
   else if (gameHandler->lives <= 0) {
      gameHandler->gameOver = true;
   }

   updateBoss();

   updateProjectilePosition(dt);

   updateEnemyPosition(dt);
}


void Window::updateBackground(double dt){
   gameHandler->bgMid = gameHandler->bgMid + gameHandler->bgSpeed * dt;
   if (gameHandler->bgMid.x >= 800) {
      gameHandler->bgMid.x = 0;
   }
}

void Window::updateBoss() {
   //spawn boss in 60 seconds
   if (gameHandler->bossTimer->getCount() >= 60) {
      gameHandler->bossExists = true;
      gameHandler->bossTimer->stopTimer();
      gameHandler->bossTimer->resetCount();
   }
}

void Window::updateProjectilePosition(double dt) {
   if (!gameHandler->lasersList.empty()) {
      for (auto it = gameHandler->lasersList.begin(); it != gameHandler->lasersList.end(); ++it) {
	      (*it)->update(dt);
      }
   }

      if (!gameHandler->missileList.empty()) {
      for (auto it = gameHandler->missileList.begin(); it != gameHandler->missileList.end(); ++it) {
	      (*it)->update(dt);
      }
   }
}

void Window::updateEnemyPosition(double dt) {
   updateEnemySpaceShipPosition(dt);
   updateBombPosition(dt);
   updateBossPosition();
   
   //spawn enemies every 5 seconds
   if(gameHandler->spawnEnemyShipsTimer->getCount() >= 5){
      gameHandler->spawnEnemies = true;
      gameHandler->spawnEnemyShipsTimer->srsTimer();
   }

   //spawn bombs every 30 seconds
   if(gameHandler->spawnBombsTimer->getCount() >= 30){
      Point pt(800, 200);
      pt.y=pt.y+(rand()%200);
      gameHandler->generateBomb = true;
      gameHandler->spawnBombsTimer->srsTimer();
   }
}

void Window::updateEnemySpaceShipPosition(double dt) {
   if (!gameHandler->enemySpaceShipsList.empty()) {
      for (auto enemy = gameHandler->enemySpaceShipsList.begin(); enemy != gameHandler->enemySpaceShipsList.end(); ++enemy) {
	      (*enemy)->update(dt);
         if((*enemy)->fire){
            int shotPosition = rand() % 2 + 1;

            if(shotPosition == 1){
               gameHandler->addLaser((*enemy)->centre, (*enemy)->color, (*enemy)->enemyLaserSpeed + Vector(0, -40));
            }else{
               gameHandler->addLaser((*enemy)->centre, (*enemy)->color, (*enemy)->enemyLaserSpeed + Vector(0, 40));
            }

            (*enemy)->fire = false;
         }
      }
   }
}

void Window::updateBombPosition(double dt) {
   if (!gameHandler->bombEnemiesList.empty()) {
      for (auto enemy = gameHandler->bombEnemiesList.begin(); enemy != gameHandler->bombEnemiesList.end(); ++enemy) {
         (*enemy)->isToUpdate = true;
         if((*enemy)->fire){
            (*enemy)->circleLaserShot = true;
            (*enemy)->fire = false;
         }
      }
   }
}

void Window::updateBossPosition() {
   if(gameHandler->boss != nullptr){
      if(!gameHandler->boss->dead){
         gameHandler->boss->isToUpdate = true;
         gameHandler->isToBossFire = true;
      }
   }
}

void Window::draw() {
   drawBackground();
   drawLives();
   drawShots();
   drawEnemySpaceShips();
   drawBombs();
   drawBoss();
   
   if (!gameHandler->gameOver && gameHandler->spaceShip) {
      drawSpaceShip();
   }
}

void Window::drawSpaceShip(){
    gameHandler->spaceShipSprite->draw_region(gameHandler->spaceShip->row, gameHandler->spaceShip->col, 47.0, 40.0, gameHandler->spaceShip->centre, 0);
}

void Window::drawBackground(){
   gameHandler->bg->draw_parallax_background(gameHandler->bgMid.x, 0);
}

void Window::drawShots() {
   if (!gameHandler->lasersList.empty()) {
      for (auto it = gameHandler->lasersList.begin(); it != gameHandler->lasersList.end(); ++it) { 
	      (*it)->draw();
      }
   }
   if (!gameHandler->missileList.empty()) {
      for (auto it = gameHandler->missileList.begin(); it != gameHandler->missileList.end(); ++it) { 
	      
          (*it)->draw();
      }
   }
}

void Window::drawEnemySpaceShips() {
   if (!gameHandler->enemySpaceShipsList.empty()) {
      for (auto enemy = gameHandler->enemySpaceShipsList.begin(); enemy != gameHandler->enemySpaceShipsList.end(); ++enemy) {
            if (!(*enemy)->dead) {
                gameHandler->enemyShip->draw_tinted((*enemy)->centre, (*enemy)->color, 0);
            }
            else {
                if ((*enemy)->dAnim < 5){
                    gameHandler->enemyDeath->draw_death_anim((*enemy)->dAnim, (*enemy)->centre, 0);
                    (*enemy)->dAnim++;
                }
                else{
                    (*enemy)->dAnim_complete = true;
                } 
            }
	   }
   }
}

void Window::drawBombs() {
   if (!gameHandler->bombEnemiesList.empty()) {
      for (auto bomb = gameHandler->bombEnemiesList.begin(); bomb != gameHandler->bombEnemiesList.end(); ++bomb) {
        if (!(*bomb)->dead) {
            gameHandler->enemyBomb->draw_region((*bomb)->row, (*bomb)->col, 40, 41, (*bomb)->centre, 0);
        }
        else {
            if ((*bomb)->dAnim < 5){
                gameHandler->enemyDeath->draw_death_anim((*bomb)->dAnim, (*bomb)->centre, 0);
                (*bomb)->dAnim++;
            }
            else{ (*bomb)->dAnim_complete = true;}
        } 
	   }
   }
}

void Window::drawBoss() {
   if (gameHandler->boss != nullptr) {
        chooseBossFrame();
        if (!gameHandler->boss->dead) {
            gameHandler->bossShip->draw_boss(gameHandler->boss->row, gameHandler->boss->col, 200, 200, gameHandler->boss->centre, 0);
        }
        else {
            if (gameHandler->boss->dAnim < 5) {
                gameHandler->enemyDeath->draw_death_anim(gameHandler->boss->dAnim, gameHandler->boss->centre, 0);
                gameHandler->boss->dAnim++;
            }
            else{
                gameHandler->boss->dAnim_complete = true;
            }
        }
   }
}

void Window::chooseBossFrame() {
   if (gameHandler->boss->lives > 50){
      gameHandler->boss->spriteSheetIndex = 0;
   }

   if (gameHandler->boss->lives <= 50 && gameHandler->boss->spriteSheetIndex < 3) { 
      gameHandler->boss->fireSpeed = rand()%50+20;
      gameHandler->boss->speed = gameHandler->boss->speed * 1.1;
      gameHandler->boss->bossSize = 70;
      gameHandler->boss->spriteSheetIndex++;
   }

   if (gameHandler->boss->lives <= 20 && gameHandler->boss->spriteSheetIndex < 8) {
      gameHandler->boss->fireSpeed = rand()%30+20;
      gameHandler->boss->speed = gameHandler->boss->speed * 1.1;
      gameHandler->boss->bossSize = 60;
      gameHandler->boss->spriteSheetIndex++;
   }

   gameHandler->boss->row = gameHandler->boss->spriteSheetIndex / 3;
   gameHandler->boss->col = gameHandler->boss->spriteSheetIndex % 3;

}

void Window::drawLives() {
   Point centre(800-70, 800-50);
   if (gameHandler->lives > 0) {
	  al_draw_rectangle(800 - 70, 50, 800 - 50, 70,
			    al_map_rgb(0, 255, 0), 5);
   }
   if (gameHandler->lives > 1) {
      al_draw_rectangle(800 - 110, 50, 800 - 90, 70,
			al_map_rgb(0, 255, 0), 5);
   }
   if (gameHandler->lives > 2) {
      al_draw_rectangle(800 - 150, 50, 800 - 130, 70,
			al_map_rgb(0, 255, 0) , 5);
   }
}
__END_API