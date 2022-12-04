/**
 * @file GameHandler.h
 */

#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <list>
#include <vector>
#include <memory>
#include <string>

#include "Vector.h"
#include "Missile.h"
#include "Laser.h"
#include "SpaceShip.h"

// forward declarations
struct Point;
class Timer;
class Enemy;
class SpaceShip;
class Sprite;
class Missile;
class Laser;

/**
 * @class GameHandler
 */

extern const int GAME_OVER_WAIT_TIME;
extern const int HIGH_SCORE_DISPLAY_TIME;
extern const int WEAPON_DELAY_LASER;
extern const int WEAPON_DELAY_MISSILE;
extern const Vector PLAYER_PROJECTILE_SPEED;

class GameHandler{
  public:
   GameHandler(int w, int h, int f);
   ~GameHandler();

   void update(double);
   void draw();
   void init();
   void input(ALLEGRO_KEYBOARD_STATE&);   
   bool is_game_over();

   // draw functions for components - called by draw
   void drawBossIncomingMessage();
   void drawLives();
   void drawBackground();
   void drawShots();
   void drawEnemies();
   void showGameOverMessage();

   // update functions for components - called by update
   void updateBoss();
   void updateBackground(double dt);
   void updateProjectilePosition(double);
   void updateEnemyPosition(double);
   void collision();
   void checkCollisionOnPlayer();
   void checkCollisionOnEnemies();
   void checkCollidingEnemyWithPlayer();
   void clean();
   void cullPlayer();
   void cleanLasers();
   void cleanMissiles();
   void cullEnemies();
   
   // add functions for game components
   void setupSpaceShip();
   void setupBackground();
   void setupTimers();
   ALLEGRO_PATH* setupPath();
   void setupSprites();
   void addLaser(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addMissile(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addCreepB(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addCreep(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addBoss(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addCreepMis(const Point&, Point, Point, Point, Point, const ALLEGRO_COLOR&,
		    const Vector&);
   void addPlayerLaserSingleShot();
   void addPlayerMissileSingleShot();
   
   // utility functions
   void spawn();
   void spawnBoss();
   void bossFire(std::shared_ptr<Enemy>);
   void CircleLaser(std::shared_ptr<Enemy>);
   bool doHitboxesIntersect(const Point&, const int&,
			    const Point&, const int&);   
   bool doColorsMatch(const ALLEGRO_COLOR&, const ALLEGRO_COLOR&);
   bool isPointBoxCollision(const Point&, const Point&, const int&);

   
  private:   
   // timers
   std::shared_ptr<Timer> gameOverTimer;
   std::shared_ptr<Timer> laserShotsTimer;
   std::shared_ptr<Timer> missileShotsTimer;
   std::shared_ptr<Timer> bossTimer;
   // base classes
   std::list< std::shared_ptr<Laser> > lasersList;
   std::list< std::shared_ptr<Missile> > missileList;
   std::list< std::shared_ptr<Enemy> > enem;   
   std::shared_ptr<SpaceShip> spaceShip;
   // sprites
   std::shared_ptr<Sprite> spaceShipSprite;
   std::shared_ptr<Sprite> enemyShip;
   std::shared_ptr<Sprite> enemyDeath;
   std::shared_ptr<Sprite> bossShip;
   std::shared_ptr<Sprite> enemyBomb;


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

   bool bossExists = false;
   bool bossIncoming = false;
   int  bossSpawnConditionCounter = 0;
   bool _Boss = false;
   bool killedBoss = false;
   bool writeComplete = false;
   bool bossFirstShot = false;


   int lives;
   int displayWidth;
   int displayHeight;
   int framesPerSec;
};

#endif
