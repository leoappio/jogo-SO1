#include "GameHandler.h"
#include "Point.h"
#include "Vector.h"
#include "SpaceShip.h"
#include "Laser.h"
#include "Missile.h"
#include "Sprite.h"
#include "Action.h"
#include "Timer.h"
#include "EnemySpaceShip.h"
#include "Bomb.h"
#include "Boss.h"
#include <iostream>
#include <fstream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

__BEGIN_API
const int GAME_OVER_WAIT_TIME = 100;
const int WEAPON_DELAY_LASER = 6;
const int WEAPON_DELAY_MISSILE = 20;
const Vector SPACE_SHIP_PROJECTILE_SPEED = Vector(500, 0);


GameHandler::GameHandler(int w, int h, int f):
   gameOver(false),
   lives(3),
   displayWidth(w),
   displayHeight(h),
   framesPerSec(f)
{
   
}

GameHandler::~GameHandler() {
   spaceShip.reset();
   bg.reset();
   lasersList.clear();
   missileList.clear();
   enemySpaceShipsList.clear();
   bombEnemiesList.clear();
   boss.reset();

   gameOverTimer.reset();
   laserShotsTimer.reset();
   missileShotsTimer.reset();
   bossTimer.reset();
   deadRespawnTimer.reset();
   spawnEnemyShipsTimer.reset();
   spawnBombsTimer.reset();
}

void GameHandler::init(){
   setupTimers();
   setupSpaceShip();
   ALLEGRO_PATH *path = setupPath();
   setupSprites();
   setupBackground();
   al_destroy_path(path);
   std::cout<<"game handler init finalizado";
}


// ---------------- SETUP FUNCTIONS ------------------
void GameHandler::setupTimers(){
   gameOverTimer = std::make_shared<Timer> (framesPerSec); gameOverTimer->create();
   laserShotsTimer = std::make_shared<Timer> (framesPerSec); laserShotsTimer->create();   
   missileShotsTimer = std::make_shared<Timer> (framesPerSec); missileShotsTimer->create();
   bossTimer = std::make_shared<Timer> (1); bossTimer->create();
   deadRespawnTimer = std::make_shared<Timer> (framesPerSec); deadRespawnTimer->create();
   spawnEnemyShipsTimer = std::make_shared<Timer> (1); spawnEnemyShipsTimer->create();
   spawnBombsTimer = std::make_shared<Timer> (1); spawnBombsTimer->create();
   bossTimer->startTimer();
   laserShotsTimer->startTimer();
   missileShotsTimer->startTimer();
   spawnEnemyShipsTimer->startTimer();
   spawnBombsTimer->startTimer();

}

void GameHandler::setupSpaceShip() {
   spaceShip = std::make_shared<SpaceShip> (Point(215, 245), al_map_rgb(0, 200, 0));
}

ALLEGRO_PATH* GameHandler::setupPath(){
   ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
   al_append_path_component(path, "resources");
   al_change_directory(al_path_cstr(path, '/'));
   return path;
}

void GameHandler::setupBackground(){
   bg = std::make_shared<Sprite> ("BGstars.png"); 
   bgMid = Point(0, 0);
   fgMid = Point(800, 0);
   fg2Mid= Point(300, 300);
   bgSpeed = Vector(50, 0);
   fgSpeed = Vector(-90, 0);
}

void GameHandler::setupSprites(){
   spaceShipSprite = std::make_shared<Sprite> ("Sprite2.png");
   enemyShip = std::make_shared<Sprite> ("EnemyBasic.png");
   enemyDeath = std::make_shared<Sprite> ("explode.png");
   enemyBomb  = std::make_shared<Sprite> ("spikebomb.png");
   bossShip   = std::make_shared<Sprite> ("bossv2.png");
}

void GameHandler::update(double dt) {
   updateBackground(dt);
   if (spaceShip) {
      spaceShip->isToUpdate = true;
      spaceShip->dtToUpdate = dt;
      //spaceShip->update(dt);
   }
   else if (!spaceShip && lives <= 0) {
      gameOver = true;
   }else{
      //respawnSpaceShip();
   }

   updateBoss();

   updateProjectilePosition(dt);

   updateEnemyPosition(dt);

   collision();

   clean();      
}


void GameHandler::updateBackground(double dt){
   bgMid = bgMid + bgSpeed * dt;
   if (bgMid.x >= 800) {
      bgMid.x = 0;
   }
}

void GameHandler::respawnSpaceShip() {
   if (!deadRespawnTimer->isRunning()) {
      deadRespawnTimer->startTimer();
   }
   if (deadRespawnTimer->getCount() > 80) {
      setupSpaceShip();
      enemySpaceShipsList.clear();
      deadRespawnTimer->stopTimer();
      deadRespawnTimer->resetCount();
   }
}


void GameHandler::draw() {
   drawBackground();
   drawLives();
   drawShots();
   drawEnemySpaceShips();
   drawBombs();
   drawBoss();
   
   if (gameOver) {
      showGameOverMessage();
   }
   else if (spaceShip) {
      spaceShip->spaceShipSprite = spaceShipSprite;
      spaceShip->isToDraw = true;
   }
}

void GameHandler::drawBackground(){
   bg->draw_parallax_background(bgMid.x, 0);
}

void GameHandler::drawShots() {
   if (!lasersList.empty()) {
      for (auto it = lasersList.begin(); it != lasersList.end(); ++it) { 
	      (*it)->draw();
      }
   }
   if (!missileList.empty()) {
      for (auto it = missileList.begin(); it != missileList.end(); ++it) { 
	      (*it)->draw();
      }
   }
}

void GameHandler::drawEnemySpaceShips() {
   if (!enemySpaceShipsList.empty()) {
      for (auto enemy = enemySpaceShipsList.begin(); enemy != enemySpaceShipsList.end(); ++enemy) {
	      (*enemy)->draw(enemyShip, enemyDeath);
	   }
   }
}

void GameHandler::drawBombs() {
   if (!bombEnemiesList.empty()) {
      for (auto bomb = bombEnemiesList.begin(); bomb != bombEnemiesList.end(); ++bomb) {
	      (*bomb)->draw(enemyBomb, enemyDeath);
	   }
   }
}

void GameHandler::drawBoss() {
   if (boss != nullptr) {
      boss->draw(bossShip, enemyDeath);
   }
}

void GameHandler::drawLives() {
   Point centre(displayWidth-70, displayWidth-50);
   if (lives > 0) {
	  al_draw_rectangle(displayWidth - 70, 50, displayWidth - 50, 70,
			    al_map_rgb(0, 255, 0), 5);
   }
   if (lives > 1) {
      al_draw_rectangle(displayWidth - 110, 50, displayWidth - 90, 70,
			al_map_rgb(0, 255, 0), 5);
   }
   if (lives > 2) {
      al_draw_rectangle(displayWidth - 150, 50, displayWidth - 130, 70,
			al_map_rgb(0, 255, 0) , 5);
   }
}

void GameHandler::showGameOverMessage() {
   if (gameOver) {
      ALLEGRO_FONT* font = al_load_font("DavidCLM-BoldItalic.ttf", 64, 0);
      al_draw_text(font, al_map_rgb(204, 3, 3), 400, 300, ALLEGRO_ALIGN_CENTRE, "GAME OVER!");
      enemySpaceShipsList.clear();
      boss = nullptr;
      bombEnemiesList.clear();
      lasersList.clear();
      missileList.clear();
   }
}
//-----------------------------



void GameHandler::input(ALLEGRO_KEYBOARD_STATE& kb) {
   if (spaceShip) {
      switch (spaceShip->input(kb)) {
	 case act::action::FIRE_PRIMARY:
	    if (laserShotsTimer->getCount() > WEAPON_DELAY_LASER) {
	      addPlayerLaserSingleShot();
	    }
	    break;
	 case act::action::FIRE_SECONDARY:
	    if (missileShotsTimer->getCount() > WEAPON_DELAY_MISSILE) {
	      addPlayerMissileSingleShot();
	    }
	    break;
	    
	 default:
	    break;
      }
   }
}


bool GameHandler::is_game_over() {
   if (gameOver && gameOverTimer->getCount() > GAME_OVER_WAIT_TIME) {
      bg->draw_parallax_background(bgMid.x, 0);
      return true;
   }
   return false;
}


void GameHandler::spawn() {
   //spawn enemies in < format if boss doesnt exist
   if(!bossExists){
      addEnemySpaceShip(Point(800, 300), al_map_rgb(246, 64, 234),Vector(-180, 0));
      addEnemySpaceShip(Point(900, 350), al_map_rgb(246, 64, 234), Vector(-180, 0));
      addEnemySpaceShip(Point(900, 250), al_map_rgb(246, 64, 234), Vector(-180, 0));
      addEnemySpaceShip(Point(1000, 400), al_map_rgb(246, 64, 234), Vector(-180, 0));
      addEnemySpaceShip(Point(1000, 200), al_map_rgb(246, 64, 234), Vector(-180, 0));
      addEnemySpaceShip(Point(1100, 100), al_map_rgb(246, 64, 234), Vector(-180, 0));
      addEnemySpaceShip(Point(1100, 500), al_map_rgb(246, 64, 234), Vector(-180, 0));
   }
}

void GameHandler::circleLaser(std::shared_ptr<Bomb> bomb) {
   
   for(int i = -500; i <= 500; i += 325) {
      for(int j = -500; j <= 500; j += 325) {
	      addLaser(bomb->centre, bomb->color, Vector(i, j));
      }
   }
   bomb->fire = false;

}
void GameHandler::bossFire(){
   Point playerloc;

   if (spaceShip) {
      playerloc = spaceShip->centre;
   }

   Vector aim(0, 0);
   aim.Angle(playerloc, boss->centre+Point(0,50), 0.9);
   addMissile(boss->centre+Point(0,50), al_map_rgb(204, 3, 3), aim, true);
   aim.Angle(playerloc, boss->centre+Point(0,-50), 0.9);
   addMissile(boss->centre+Point(0,-50), al_map_rgb(204, 3, 3), aim, true);
}

void GameHandler::spawnBoss() {
   addBoss(Point(850, 300), al_map_rgb(155, 0, 0), Vector(-50, 0));
   bossExists = true;

}

void GameHandler::addBoss(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   boss = std::make_shared<Boss> (cen, col, spd);
}

void GameHandler::updateBoss() {
   if (bossTimer->getCount() >= 60) {
      spawnBoss();
      bossTimer->stopTimer();
      bossTimer->resetCount();
   }
}

void GameHandler::updateProjectilePosition(double dt) {
   if (!lasersList.empty()) {
      for (auto it = lasersList.begin(); it != lasersList.end(); ++it) {
	      (*it)->update(dt);
      }
   }

      if (!missileList.empty()) {
      for (auto it = missileList.begin(); it != missileList.end(); ++it) {
	      (*it)->update(dt);
      }
   }
}

void GameHandler::updateEnemyPosition(double dt) {
   updateEnemySpaceShipPosition(dt);
   updateBombPosition(dt);
   updateBossPosition(dt);
   
   //spawn enemies every 5 seconds
   if(spawnEnemyShipsTimer->getCount() >= 5){
      spawn();
      spawnEnemyShipsTimer->srsTimer();
   }

   //spawn bombs every 30 seconds
   if(spawnBombsTimer->getCount() >= 30){
      Point pt(800, 200);
      pt.y=pt.y+(rand()%200);
      addBomb(pt, al_map_rgb(204,3,3), Vector(-60, 0));
      spawnBombsTimer->srsTimer();
   }
}

void GameHandler::updateEnemySpaceShipPosition(double dt) {
   if (!enemySpaceShipsList.empty()) {
      for (auto enemy = enemySpaceShipsList.begin(); enemy != enemySpaceShipsList.end(); ++enemy) {
	      (*enemy)->update(dt);
         if((*enemy)->fire){
            int shotPosition = rand() % 2 + 1;

            if(shotPosition == 1){
               addLaser((*enemy)->centre, (*enemy)->color, (*enemy)->enemyLaserSpeed + Vector(0, -40));
            }else{
               addLaser((*enemy)->centre, (*enemy)->color, (*enemy)->enemyLaserSpeed + Vector(0, 40));
            }

            (*enemy)->fire = false;
         }
      }
   }
}

void GameHandler::updateBombPosition(double dt) {
   if (!bombEnemiesList.empty()) {
      for (auto enemy = bombEnemiesList.begin(); enemy != bombEnemiesList.end(); ++enemy) {
         (*enemy)->update(dt);
         if((*enemy)->fire){
            circleLaser((*enemy));
            (*enemy)->fire = false;
         }
      }
   }
}

void GameHandler::updateBossPosition(double dt) {
   if(boss != nullptr){
      if(!boss->dead){
         boss->update(dt);
         if(boss->fire){
            bossFire();
         }
      }
   }
}


void GameHandler::collision() {
   checkCollisionOnPlayer();
   checkCollisionOnEnemies();
   checkCollidingEnemyWithPlayer();
}

void GameHandler::checkCollisionOnEnemies() {

   if(!lasersList.empty() && spaceShip){
      for (std::list< std::shared_ptr<Laser> >::iterator it_laser = 
	      lasersList.begin(); it_laser != lasersList.end(); ++it_laser) {
         if (doColorsMatch(spaceShip->color, (*it_laser)->color)) {
            for (std::list< std::shared_ptr<EnemySpaceShip> >::iterator it_enemy_SS = 
               enemySpaceShipsList.begin(); it_enemy_SS != enemySpaceShipsList.end(); ++it_enemy_SS) {
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
               bombEnemiesList.begin(); it_bomb != bombEnemiesList.end(); ++it_bomb) {
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

            if(boss != nullptr){
               Point pt_proj = (*it_laser)->centre;
               Point pt_enem = boss->centre;
               int enem_size = boss->bossSize;

               if ((pt_proj.x > pt_enem.x - enem_size) &&
               (pt_proj.x < pt_enem.x + enem_size) &&
               (pt_proj.y > pt_enem.y - enem_size) &&
               (pt_proj.y < pt_enem.y + enem_size)) {

                  (*it_laser)->live = false;
                  boss->hit();
               }
            }
         }
      }
   }
   
   if (!missileList.empty() && spaceShip){
      for (std::list< std::shared_ptr<Missile> >::iterator it_missile = 
	      missileList.begin(); it_missile != missileList.end(); ++it_missile) {
         if (doColorsMatch(spaceShip->color, (*it_missile)->color)) {
            for (std::list< std::shared_ptr<EnemySpaceShip> >::iterator it_enemy_SS = 
               enemySpaceShipsList.begin(); it_enemy_SS != enemySpaceShipsList.end(); ++it_enemy_SS) {
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
               bombEnemiesList.begin(); it_bomb != bombEnemiesList.end(); ++it_bomb) {
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

            if(boss != nullptr){
               Point pt_proj = (*it_missile)->centre;
               Point pt_enem = boss->centre;
               int enem_size = boss->bossSize;

               if ((pt_proj.x > pt_enem.x - enem_size) &&
               (pt_proj.x < pt_enem.x + enem_size) &&
               (pt_proj.y > pt_enem.y - enem_size) &&
               (pt_proj.y < pt_enem.y + enem_size)) {

                  boss->hit();
               }
            }
         }
      }
   }
}

void GameHandler::checkCollidingEnemyWithPlayer() {

   if(spaceShip){
      for (std::list< std::shared_ptr<EnemySpaceShip> >::iterator it_enemy_SS = 
         enemySpaceShipsList.begin(); it_enemy_SS != enemySpaceShipsList.end(); ++it_enemy_SS) {
         if (doHitboxesIntersect(spaceShip->centre, 16, (*it_enemy_SS)->centre, (*it_enemy_SS)->size)) {
	            (*it_enemy_SS)->hit();
               spaceShip->isToHit = true;
               spaceShip->hitValue = 1;
         }
      }

      for (std::list< std::shared_ptr<Bomb> >::iterator it_bomb = 
         bombEnemiesList.begin(); it_bomb != bombEnemiesList.end(); ++it_bomb) {
            if (doHitboxesIntersect(spaceShip->centre, 16, (*it_bomb)->centre, (*it_bomb)->size)) {
               (*it_bomb)->hit();
               spaceShip->isToHit = true;
               spaceShip->hitValue = 1;
            }
      }

      if(boss != nullptr){
         if (doHitboxesIntersect(spaceShip->centre, 16, boss->centre, boss->bossSize)) {
            boss->hit();
            spaceShip->isToHit = true;
            spaceShip->hitValue = 1;
         }
      }
   }   
}


void GameHandler::checkCollisionOnPlayer() {
   if(spaceShip){
      for (std::list< std::shared_ptr<Laser> >::iterator it_laser = 
	      lasersList.begin(); it_laser != lasersList.end(); ++it_laser) {
         if (!doColorsMatch((*it_laser)->color, spaceShip->color)) {
            if (isPointBoxCollision((*it_laser)->centre, spaceShip->centre, 16)) {	  
               (*it_laser)->live = false;
               spaceShip->isToHit = true;
               spaceShip->hitValue = 1;
            }
         }
      }
   }

   if (spaceShip){
      for (std::list< std::shared_ptr<Missile> >::iterator it_missile = 
	      missileList.begin(); it_missile != missileList.end(); ++it_missile) {
         if (!doColorsMatch((*it_missile)->color, spaceShip->color)) {
            if (isPointBoxCollision((*it_missile)->centre, spaceShip->centre, 16)) {
               spaceShip->isToHit = true;
               spaceShip->hitValue = 1;
            } 
         }
      }
   }
}

void GameHandler::clean() {
   cullPlayer();
   cleanLasers();
   cleanMissiles();
   cullEnemies();
}

void GameHandler::cullPlayer() {
   if (spaceShip && spaceShip->dead) {
      lives -= 1;
      spaceShip.reset();
   }
}

void GameHandler::cleanLasers() {
   std::list< std::shared_ptr<Laser> > newLaser;
   if (!lasersList.empty()) {
      for (auto it = lasersList.begin(); 
	   it != lasersList.end(); ++it) {
	 if ((*it)->live)
	    newLaser.push_back(*it);
      }
      lasersList.clear();
      lasersList.assign(newLaser.begin(), newLaser.end());      
   }
}

void GameHandler::cleanMissiles() {
   std::list< std::shared_ptr<Missile> > newMissile;
   if (!missileList.empty()) {
      for (auto it = missileList.begin(); 
	   it != missileList.end(); ++it) {
	 if ((*it)->live)
	    newMissile.push_back(*it);
      }
      missileList.clear();
      missileList.assign(newMissile.begin(), newMissile.end());      
   }
}

void GameHandler::cullEnemies() {
   std::list< std::shared_ptr<EnemySpaceShip>> livedEnemies;
   if(!enemySpaceShipsList.empty()){
      for (auto enemy = enemySpaceShipsList.begin(); enemy != enemySpaceShipsList.end(); ++enemy) {
         if (!(*enemy)->dAnim_complete) {
	         livedEnemies.push_back(*enemy);
	      }
      }
      enemySpaceShipsList.clear();
      enemySpaceShipsList.assign(livedEnemies.begin(), livedEnemies.end());
   }
   
   std::list< std::shared_ptr<Bomb>> livedBombs;
   if(!bombEnemiesList.empty()){
      for (auto bomb = bombEnemiesList.begin(); bomb != bombEnemiesList.end(); ++bomb) {
         if (!(*bomb)->dAnim_complete) {
	         livedBombs.push_back(*bomb);
	      }
      }
      bombEnemiesList.clear();
      bombEnemiesList.assign(livedBombs.begin(), livedBombs.end());
   }

   if (boss != nullptr){
      if(boss->dAnim_complete && bossExists){
         if(lives<3){
            lives++;
            bossExists = false;
         }
      }
   }
}

void GameHandler::addLaser(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   lasersList.push_back(std::make_shared<Laser> (cen, col, spd));
}

void GameHandler::addMissile(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd, bool isFromBoss) {
   std::shared_ptr<Missile> missileObject = std::make_shared<Missile> (cen, col, spd, isFromBoss);
   missileObject->load_assets();
   missileList.push_back(missileObject);
}

void GameHandler::addEnemySpaceShip(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   enemySpaceShipsList.push_back(std::make_shared<EnemySpaceShip> (cen, col, spd));
}

void GameHandler::addBomb(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   bombEnemiesList.push_back(std::make_shared<Bomb>(cen, col, spd));
}

void GameHandler::addPlayerLaserSingleShot() {
   addLaser(spaceShip->centre + Point(-20, 0), spaceShip->color, SPACE_SHIP_PROJECTILE_SPEED);
   laserShotsTimer->srsTimer();
}

void GameHandler::addPlayerMissileSingleShot() {
   addMissile(spaceShip->centre, spaceShip->color, SPACE_SHIP_PROJECTILE_SPEED, false);
   missileShotsTimer->srsTimer();
}

bool GameHandler::isPointBoxCollision(const Point& p1, const Point& p2, const int& s2) {
   return ((p1.x > p2.x - s2) &&
	   (p1.x < p2.x + s2) &&
	   (p1.y > p2.y - s2) &&
	   (p1.y < p2.y + s2));
}

bool GameHandler::doColorsMatch(const ALLEGRO_COLOR& a, const ALLEGRO_COLOR& b) {
   return (a.r == b.r && a.g == b.g && a.b == b.b);
}


bool GameHandler::doHitboxesIntersect(const Point& centre1, const int& size1,
	const Point& centre2, const int& size2) {
   return (abs(centre1.x - centre2.x) < (size1 + size2) &&
	   abs(centre1.y - centre2.y) < (size1 + size2));
}

__END_API