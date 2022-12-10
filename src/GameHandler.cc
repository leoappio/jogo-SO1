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


GameHandler::GameHandler():
   gameOver(false),
   generateBomb(false),
   spawnEnemies(false),
   isToUpdate(false),
   isToDraw(false),
   bossExists(false),
   isToBossFire(false),
   lives(3),
   displayWidth(800),
   displayHeight(600),
   framesPerSec(60),
   dtToUpdate(0.0),
   lastAction(act::action::NO_ACTION)
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


void GameHandler::init() {
   al_init();
   if ((_display = al_create_display(800, 600)) == NULL) {
      std::cout << "Cannot initialize the display\n";
      exit(1); 
   }   
   // initialize addons
   al_init_primitives_addon();
   al_init_font_addon();
   al_init_ttf_addon();
   al_init_image_addon();
   // initialize our timers
   if ((_timer = al_create_timer(1.0 / 60)) == NULL) {
      std::cout << "error, could not create timer\n";
      exit(1);
   }
   if ((_eventQueue = al_create_event_queue()) == NULL) {
      std::cout << "error, could not create event queue\n";
      exit(1);
   }
   // register our allegro _eventQueue
   al_register_event_source(_eventQueue, al_get_display_event_source(_display)); 
   al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));
   al_start_timer(_timer);
   // install keyboard
   if (!al_install_keyboard()) {
      std::cerr << "Could not install keyboard\n";
   }
   
   // register keyboard
   al_register_event_source(_eventQueue, al_get_keyboard_event_source());

   setupTimers();
   setupSpaceShip();
   ALLEGRO_PATH *path = setupPath();
   setupSprites();
   setupBackground();
   al_destroy_path(path);
   std::cout<<"game handler init finalizado";
}


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