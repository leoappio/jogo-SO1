#include "CollisionAndLife.h"
#include "EnemySpaceShip.h"

__BEGIN_API

void CollisionAndLife::run(){
    while(!gameHandler->gameOver){
        checkCollisionOnPlayer();
        checkCollisionOnEnemies();
        checkCollidingEnemyWithPlayer();
        clean();
        Thread::yield();
    }
}

void CollisionAndLife::checkCollisionOnEnemies() {

   if(!gameHandler->lasersList.empty() && gameHandler->spaceShip){
      for (std::list< std::shared_ptr<Laser> >::iterator it_laser = 
	      gameHandler->lasersList.begin(); it_laser != gameHandler->lasersList.end(); ++it_laser) {
         if (doColorsMatch(gameHandler->spaceShip->color, (*it_laser)->color)) {
            for (std::list< std::shared_ptr<EnemySpaceShip> >::iterator it_enemy_SS = 
               gameHandler->enemySpaceShipsList.begin(); it_enemy_SS != gameHandler->enemySpaceShipsList.end(); ++it_enemy_SS) {
               Point pt_proj = (*it_laser)->centre;
               Point pt_enem = (*it_enemy_SS)->centre;
               int enem_size = (*it_enemy_SS)->size;

               if ((pt_proj.x > pt_enem.x - enem_size) &&
               (pt_proj.x < pt_enem.x + enem_size) &&
               (pt_proj.y > pt_enem.y - enem_size) &&
               (pt_proj.y < pt_enem.y + enem_size)) {

                  (*it_laser)->live = false;
                  (*it_enemy_SS)->hit();
               }
            }

            for (std::list< std::shared_ptr<Bomb> >::iterator it_bomb = 
               gameHandler->bombEnemiesList.begin(); it_bomb != gameHandler->bombEnemiesList.end(); ++it_bomb) {
               Point pt_proj = (*it_laser)->centre;
               Point pt_enem = (*it_bomb)->centre;
               int enem_size = (*it_bomb)->size;

               if ((pt_proj.x > pt_enem.x - enem_size) &&
               (pt_proj.x < pt_enem.x + enem_size) &&
               (pt_proj.y > pt_enem.y - enem_size) &&
               (pt_proj.y < pt_enem.y + enem_size)) {

                  (*it_laser)->live = false;
                  (*it_bomb)->hit();
               }
            }

            if(gameHandler->boss != nullptr){
               Point pt_proj = (*it_laser)->centre;
               Point pt_enem = gameHandler->boss->centre;
               int enem_size = gameHandler->boss->bossSize;

               if ((pt_proj.x > pt_enem.x - enem_size) &&
               (pt_proj.x < pt_enem.x + enem_size) &&
               (pt_proj.y > pt_enem.y - enem_size) &&
               (pt_proj.y < pt_enem.y + enem_size)) {

                  (*it_laser)->live = false;
                  gameHandler->boss->hit();
               }
            }
         }
      }
   }
   
   if (!gameHandler->missileList.empty() && gameHandler->spaceShip){
      for (std::list< std::shared_ptr<Missile> >::iterator it_missile = 
	      gameHandler->missileList.begin(); it_missile != gameHandler->missileList.end(); ++it_missile) {
         if (doColorsMatch(gameHandler->spaceShip->color, (*it_missile)->color)) {
            for (std::list< std::shared_ptr<EnemySpaceShip> >::iterator it_enemy_SS = 
               gameHandler->enemySpaceShipsList.begin(); it_enemy_SS != gameHandler->enemySpaceShipsList.end(); ++it_enemy_SS) {
               Point pt_proj = (*it_missile)->centre;
               Point pt_enem = (*it_enemy_SS)->centre;
               int enem_size = (*it_enemy_SS)->size;

               if ((pt_proj.x > pt_enem.x - enem_size) &&
               (pt_proj.x < pt_enem.x + enem_size) &&
               (pt_proj.y > pt_enem.y - enem_size) &&
               (pt_proj.y < pt_enem.y + enem_size)) {
                  (*it_enemy_SS)->hit();
               }
            }

            for (std::list< std::shared_ptr<Bomb> >::iterator it_bomb = 
               gameHandler->bombEnemiesList.begin(); it_bomb != gameHandler->bombEnemiesList.end(); ++it_bomb) {
               Point pt_proj = (*it_missile)->centre;
               Point pt_enem = (*it_bomb)->centre;
               int enem_size = (*it_bomb)->size;

               if ((pt_proj.x > pt_enem.x - enem_size) &&
               (pt_proj.x < pt_enem.x + enem_size) &&
               (pt_proj.y > pt_enem.y - enem_size) &&
               (pt_proj.y < pt_enem.y + enem_size)) {

                  (*it_bomb)->hit();
               }
            }

            if(gameHandler->boss != nullptr){
               Point pt_proj = (*it_missile)->centre;
               Point pt_enem = gameHandler->boss->centre;
               int enem_size = gameHandler->boss->bossSize;

               if ((pt_proj.x > pt_enem.x - enem_size) &&
               (pt_proj.x < pt_enem.x + enem_size) &&
               (pt_proj.y > pt_enem.y - enem_size) &&
               (pt_proj.y < pt_enem.y + enem_size)) {

                  gameHandler->boss->hit();
               }
            }
         }
      }
   }
}

void CollisionAndLife::checkCollidingEnemyWithPlayer() {

   if(gameHandler->spaceShip){
      for (std::list< std::shared_ptr<EnemySpaceShip> >::iterator it_enemy_SS = 
         gameHandler->enemySpaceShipsList.begin(); it_enemy_SS != gameHandler->enemySpaceShipsList.end(); ++it_enemy_SS) {
         if (doHitboxesIntersect(gameHandler->spaceShip->centre, 16, (*it_enemy_SS)->centre, (*it_enemy_SS)->size)) {
	            (*it_enemy_SS)->hit();
               gameHandler->spaceShip->isToHit = true;
               gameHandler->spaceShip->hitValue = 3;
         }
      }

      for (std::list< std::shared_ptr<Bomb> >::iterator it_bomb = 
         gameHandler->bombEnemiesList.begin(); it_bomb != gameHandler->bombEnemiesList.end(); ++it_bomb) {
            if (doHitboxesIntersect(gameHandler->spaceShip->centre, 16, (*it_bomb)->centre, (*it_bomb)->size)) {
               (*it_bomb)->hit();
               gameHandler->spaceShip->isToHit = true;
               gameHandler->spaceShip->hitValue = 3;
            }
      }

      if(gameHandler->boss != nullptr){
         if (doHitboxesIntersect(gameHandler->spaceShip->centre, 16, gameHandler->boss->centre, gameHandler->boss->bossSize)) {
            gameHandler->boss->hit();
            gameHandler->spaceShip->isToHit = true;
            gameHandler->spaceShip->hitValue = 3;
         }
      }
   }   
}


void CollisionAndLife::checkCollisionOnPlayer() {
   if(gameHandler->spaceShip){
      for (std::list< std::shared_ptr<Laser> >::iterator it_laser = 
	      gameHandler->lasersList.begin(); it_laser != gameHandler->lasersList.end(); ++it_laser) {
         if (!doColorsMatch((*it_laser)->color, gameHandler->spaceShip->color)) {
            if (isPointBoxCollision((*it_laser)->centre, gameHandler->spaceShip->centre, 16)) {	  
               (*it_laser)->live = false;
               gameHandler->spaceShip->isToHit = true;
               gameHandler->spaceShip->hitValue = 1;
            }
         }
      }
   }

   if (gameHandler->spaceShip){
      for (std::list< std::shared_ptr<Missile> >::iterator it_missile = 
	      gameHandler->missileList.begin(); it_missile != gameHandler->missileList.end(); ++it_missile) {
         if (!doColorsMatch((*it_missile)->color, gameHandler->spaceShip->color)) {
            if (isPointBoxCollision((*it_missile)->centre, gameHandler->spaceShip->centre, 16)) {
               gameHandler->spaceShip->isToHit = true;
               gameHandler->spaceShip->hitValue = 3;
            } 
         }
      }
   }
}

void CollisionAndLife::clean() {
   cullPlayer();
   cleanLasers();
   cleanMissiles();
   cullEnemies();
}

void CollisionAndLife::cullPlayer() {
   if (gameHandler->spaceShip && gameHandler->spaceShip->dead) {
      gameHandler->lives -= 1;
      //gameHandler->spaceShip.reset();
   }
}

void CollisionAndLife::cleanLasers() {
   std::list< std::shared_ptr<Laser> > newLaser;
   if (!gameHandler->lasersList.empty()) {
      for (auto it = gameHandler->lasersList.begin(); 
	   it != gameHandler->lasersList.end(); ++it) {
	 if ((*it)->live)
	    newLaser.push_back(*it);
      }
      gameHandler->lasersList.clear();
      gameHandler->lasersList.assign(newLaser.begin(), newLaser.end());      
   }
}

void CollisionAndLife::cleanMissiles() {
   std::list< std::shared_ptr<Missile> > newMissile;
   if (!gameHandler->missileList.empty()) {
      for (auto it = gameHandler->missileList.begin(); 
	   it != gameHandler->missileList.end(); ++it) {
	 if ((*it)->live)
	    newMissile.push_back(*it);
      }
      gameHandler->missileList.clear();
      gameHandler->missileList.assign(newMissile.begin(), newMissile.end());      
   }
}

void CollisionAndLife::cullEnemies() {
   std::list< std::shared_ptr<EnemySpaceShip>> livedEnemies;
   if(!gameHandler->enemySpaceShipsList.empty()){
      for (auto enemy = gameHandler->enemySpaceShipsList.begin(); enemy != gameHandler->enemySpaceShipsList.end(); ++enemy) {
         if (!(*enemy)->dAnim_complete) {
	        livedEnemies.push_back(*enemy);
	      }
      }
      gameHandler->enemySpaceShipsList.clear();
      gameHandler->enemySpaceShipsList.assign(livedEnemies.begin(), livedEnemies.end());
   }
   
   std::list< std::shared_ptr<Bomb>> livedBombs;
   if(!gameHandler->bombEnemiesList.empty()){
      for (auto bomb = gameHandler->bombEnemiesList.begin(); bomb != gameHandler->bombEnemiesList.end(); ++bomb) {
         if (!(*bomb)->dAnim_complete) {
	        livedBombs.push_back(*bomb);
	      }
      }
      gameHandler->bombEnemiesList.clear();
      gameHandler->bombEnemiesList.assign(livedBombs.begin(), livedBombs.end());
   }

   if (gameHandler->boss != nullptr){
      if(gameHandler->boss->dAnim_complete && gameHandler->bossExists){
         if(gameHandler->lives<3){
            gameHandler->lives++;
            gameHandler->bossExists = false;
         }
      }
   }
}

bool CollisionAndLife::isPointBoxCollision(const Point& p1, const Point& p2, const int& s2) {
   return ((p1.x > p2.x - s2) &&
	   (p1.x < p2.x + s2) &&
	   (p1.y > p2.y - s2) &&
	   (p1.y < p2.y + s2));
}

bool CollisionAndLife::doColorsMatch(const ALLEGRO_COLOR& a, const ALLEGRO_COLOR& b) {
   return (a.r == b.r && a.g == b.g && a.b == b.b);
}


bool CollisionAndLife::doHitboxesIntersect(const Point& centre1, const int& size1,
	const Point& centre2, const int& size2) {
   return (abs(centre1.x - centre2.x) < (size1 + size2) &&
	   abs(centre1.y - centre2.y) < (size1 + size2));
}
__END_API