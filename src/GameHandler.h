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

extern const int GAME_OVER_WAIT_TIME;
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

   void drawBossIncomingMessage();
   void drawLives();
   void drawBackground();
   void drawShots();
   void drawEnemySpaceShips();
   void drawBombs();
   void drawBoss();
   void showGameOverMessage();

   void updateBoss();
   void updateBackground(double dt);
   void updateProjectilePosition(double);
   void updateEnemyPosition(double);
   void updateEnemySpaceShipPosition(double dt);
   void updateBombPosition(double dt);
   void updateBossPosition(double dt);
   
   void respawnSpaceShip();


   void collision();
   void checkCollisionOnPlayer();
   void checkCollisionOnEnemies();
   void checkCollidingEnemyWithPlayer();
   void clean();
   void cullPlayer();
   void cleanLasers();
   void cleanMissiles();
   void cullEnemies();
   
   void setupSpaceShip();
   void setupBackground();
   void setupTimers();
   ALLEGRO_PATH* setupPath();
   void setupSprites();


   void addLaser(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addMissile(const Point&, const ALLEGRO_COLOR&, const Vector&, bool isFromBoss);
   void addBomb(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addEnemySpaceShip(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addBoss(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addPlayerLaserSingleShot();
   void addPlayerMissileSingleShot();
   
   void spawn();
   void spawnBoss();
   void bossFire();
   void circleLaser(std::shared_ptr<Bomb> bomb);
   bool doHitboxesIntersect(const Point&, const int&,
			    const Point&, const int&);   
   bool doColorsMatch(const ALLEGRO_COLOR&, const ALLEGRO_COLOR&);
   bool isPointBoxCollision(const Point&, const Point&, const int&);

  
   std::shared_ptr<Timer> gameOverTimer;
   std::shared_ptr<Timer> laserShotsTimer;
   std::shared_ptr<Timer> missileShotsTimer;
   std::shared_ptr<Timer> bossTimer;
   std::shared_ptr<Timer> deadRespawnTimer;
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

   private:   
   bool bossExists = false;
   bool _Boss = false;

   int lives;
   int displayWidth;
   int displayHeight;
   int framesPerSec;
};
__END_API
#endif
