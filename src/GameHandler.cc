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
GameHandler::GameHandler():
   gameOver(false),
   generateBomb(false),
   spawnEnemies(false),
   isToUpdate(false),
   isToDraw(false),
   bossExists(false),
   isToBossFire(false),
   lives(3),
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
   gameOverTimer = std::make_shared<Timer> (60); gameOverTimer->create();
   laserShotsTimer = std::make_shared<Timer> (60); laserShotsTimer->create();   
   missileShotsTimer = std::make_shared<Timer> (60); missileShotsTimer->create();
   bossTimer = std::make_shared<Timer> (1); bossTimer->create();
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

void GameHandler::circleLaser(std::shared_ptr<Bomb> bomb) {
   
   for(int i = -500; i <= 500; i += 325) {
      for(int j = -500; j <= 500; j += 325) {
	      addLaser(bomb->centre, bomb->color, Vector(i, j));
      }
   }
   bomb->fire = false;

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
   addLaser(spaceShip->centre + Point(-20, 0), spaceShip->color, Vector(500, 0));
   laserShotsTimer->srsTimer();
}

void GameHandler::addPlayerMissileSingleShot() {
   addMissile(spaceShip->centre, spaceShip->color, Vector(500, 0), false);
   missileShotsTimer->srsTimer();
}

__END_API