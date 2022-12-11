#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "Vector.h"
#include "Missile.h"
#include "Laser.h"
#include "SpaceShip.h"
#include "Bomb.h"
#include "Boss.h"
#include <list>
#include <vector>
#include <memory>
#include <string>

__BEGIN_API

class GameHandler{
  public:
   GameHandler();
   ~GameHandler();

   void init();

   void setupSpaceShip();
   void setupBackground();
   void setupTimers();
   ALLEGRO_PATH* setupPath();
   void setupSprites();


   void addLaser(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addMissile(const Point&, const ALLEGRO_COLOR&, const Vector&, bool isFromBoss);
   void addBomb(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addEnemySpaceShip(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addPlayerLaserSingleShot();
   void addPlayerMissileSingleShot();   

   ALLEGRO_TIMER *_timer;
   ALLEGRO_EVENT_QUEUE *_eventQueue;
   ALLEGRO_DISPLAY *_display;

   std::shared_ptr<Timer> gameOverTimer;
   std::shared_ptr<Timer> laserShotsTimer;
   std::shared_ptr<Timer> missileShotsTimer;
   std::shared_ptr<Timer> bossTimer;
   std::shared_ptr<Timer> spawnEnemyShipsTimer;
   std::shared_ptr<Timer> spawnBombsTimer;

   std::list<std::shared_ptr<Laser>> lasersList;
   std::list<std::shared_ptr<Missile>> missileList;
   std::list<std::shared_ptr<EnemySpaceShip>> enemySpaceShipsList;
   std::list<std::shared_ptr<Bomb>> bombEnemiesList;
   std::shared_ptr<SpaceShip> spaceShip;

   std::shared_ptr<Sprite> spaceShipSprite;
   std::shared_ptr<Sprite> enemyShip;
   std::shared_ptr<Sprite> enemyDeath;
   std::shared_ptr<Sprite> bossShip;
   std::shared_ptr<Sprite> enemyBomb;
   std::shared_ptr<Boss> boss;

   //Background
   Point bgMid;
   Point fgMid;
   Point fg2Mid;
   Vector bgSpeed;
   Vector fgSpeed;
   std::shared_ptr<Sprite> bg;
   std::shared_ptr<Sprite> fg;
   
   // flags
   bool gameOver;
   bool generateBomb;
   bool spawnEnemies;
   bool isToUpdate;
   bool isToDraw;
   std::list<act::action> actionsList;
   bool bossExists;
   bool isToBossFire;

   int lives;
   double dtToUpdate;
};
__END_API
#endif
