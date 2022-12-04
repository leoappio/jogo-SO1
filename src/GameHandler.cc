/**
 * @file GameHandler.cc
 */

#include "GameHandler.h"
#include <iostream>
#include <fstream>
#include "Point.h"
#include "Vector.h"
#include "SpaceShip.h"
#include "Laser.h"
#include "Missile.h"
#include "Sprite.h"
#include "Action.h"
#include "Timer.h"
#include "Enemy.h"
#include "Creep.h"
#include "CreepMis.h"
#include "CreepBomb.h"
#include "Boss.h"

const int GAME_OVER_WAIT_TIME = 100;
const int HIGH_SCORE_DISPLAY_TIME = 200;
const int WEAPON_DELAY_LASER = 6;
const int WEAPON_DELAY_MISSILE = 20;
const Vector SPACE_SHIP_PROJECTILE_SPEED = Vector(500, 0);


GameHandler::GameHandler(int w, int h, int f):
   gameOver(false),
   lives(3),
   framesPerSec(f)
{
   
}

GameHandler::~GameHandler() {
   spaceShip.reset();
   bg.reset();
   lasersList.clear();
   missileList.clear();
   enem.clear(); //TODO: ALTERAR ENEMIES PARA CLASSES ESPECIFICIAS

   gameOverTimer.reset();
   laserShotsTimer.reset();
   missileShotsTimer.reset();
}

void GameHandler::init() {
   setupTimers();
   setupSpaceShip();
   ALLEGRO_PATH *path = setupPath();
   setupBackground();
   setupSprites();
   al_destroy_path(path);
}


// ---------------- SETUP FUNCTIONS ------------------
void GameHandler::setupTimers(){
   gameOverTimer = std::make_shared<Timer> (framesPerSec); gameOverTimer->create();
   laserShotsTimer = std::make_shared<Timer> (framesPerSec); laserShotsTimer->create();   
   missileShotsTimer = std::make_shared<Timer> (framesPerSec); missileShotsTimer->create();
   bossTimer = std::make_shared<Timer> (framesPerSec); bossTimer->create();
   laserShotsTimer->startTimer();
   missileShotsTimer->startTimer();

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

//-------------UPDATE FUNCTIONS---------------
void GameHandler::update(double dt) {
   updateBackground(dt);
   if (spaceShip) {
      spaceShip->update(dt);
   }
   else if (!spaceShip && lives <= 0) {
      gameOver = true;
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

//-------------draw functions----------------
void GameHandler::draw() {
   drawBackground();
   drawLives();
   drawShots();
   drawEnemies(); 
   drawBossIncomingMessage();
   
   if (gameOver) {
      showGameOverMessage();
   }
   else if (spaceShip) {
      spaceShip->draw(spaceShipSprite, 0);
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

void GameHandler::drawEnemies() {
   if (!enem.empty()) {
      for (auto it = enem.begin(); it != enem.end(); ++it) {
	 if (doColorsMatch((*it)->getColor(), al_map_rgb(204, 3, 3))) {
	    // bomb enemy
	    (*it)->draw(enemyBomb, enemyDeath);
	 }
	 else if (doColorsMatch((*it)->getColor(), al_map_rgb(155, 0, 0))) {
	    // boss enemy
	    (*it)->draw(bossShip, enemyDeath);
	 }				
	 else {
	    // creep enemy
	    (*it)->draw(enemyShip, enemyDeath);
	 }
      }
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
   if (!spaceShip && lives > 0) {
      //gameOverFont->drawTextCenteredF(al_map_rgb(255, 0, 0), "%i LIVES REMAINING", lives);
   }
}

void GameHandler::showGameOverMessage() {
   if (!gameOverTimer->isRunning()) {
      gameOverTimer->startTimer();
   }
   if (gameOverTimer->getCount() < GAME_OVER_WAIT_TIME) {
      // game over message
      //gameOverFont->drawTextCentered(al_map_rgb(255, 0, 0), "GAME OVER");
   }   
}
//-----------------------------



void GameHandler::input(ALLEGRO_KEYBOARD_STATE& kb) {
   if (spaceShip) {
      switch (spaceShip->input(kb)) {
	 case act::action::FIRE_PRIMARY:
	    if (laserShotsTimer->getCount() > WEAPON_DELAY_LASER) {
         std::cout << "tiro normal\n";
	      addPlayerLaserSingleShot();
	    }
	    break;
	 case act::action::FIRE_SECONDARY:
	    if (missileShotsTimer->getCount() > WEAPON_DELAY_MISSILE) {
         std::cout << "missel\n";
	      addPlayerMissileSingleShot();
	    }
	    break;
	    
	 default:
	    break;
      }
   }
}


bool GameHandler::is_game_over() {
   // change this condition------------
   if (gameOver && gameOverTimer->getCount() > GAME_OVER_WAIT_TIME + HIGH_SCORE_DISPLAY_TIME) {
      bg->draw_parallax_background(bgMid.x, 0);
      return true;
   }
   return false;
}


void GameHandler::spawn() {
   Point pt(800, 200);
   Point pt1, pt2, playerloc, pt3;
   Vector spd(0, 0);
   if (spaceShip)
      playerloc = spaceShip->centre;
   else
      playerloc = Point (200, 300);

   int n = rand() % 6 + 1;

   //ENEMIES SPAWN
   
   // select enemy routine
   switch(n) {      
      case 1: // wave of 5
	 for (int i = 0; i < 5; i++) {
	    pt.rollRandom();
	    spd.rollRandom();
	    addCreep(pt, al_map_rgb(255, 51, 51), spd);
	 }	 
	 break;
	 
      case 2: // wave of 8
	 for (int i = 0; i < 8; i++) {
	    pt.rollRandom();
	    spd.rollRandom();
	    addCreep(pt, al_map_rgb(255, 159, 48), spd);
	 }
	 break;
	 
      case 3: // SPAWN EM V
	 addCreep(Point(800, 300), al_map_rgb(246, 64, 234),Vector(-180, 0));
	 addCreep(Point(900, 350), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 addCreep(Point(900, 250), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 addCreep(Point(1000, 400), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 addCreep(Point(1000, 200), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 addCreep(Point(1100, 100), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 addCreep(Point(1100, 500), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 break;
      
      case 4: // 3 enemies that track to initial position of player
	 pt1.x=800; pt1.y=580;
	 pt2.x=800; pt2.y=20;
	 pt3.x=850; pt3.y=300;
	 spd.Angle(playerloc, pt1, 1);
	 addCreep(pt1, al_map_rgb(255, 255, 255),spd);
	 spd.Angle(playerloc, pt2, 1);
	 addCreep(pt2, al_map_rgb(255, 255, 255),spd);
	 spd.Angle(playerloc, pt3, 1);
	 addCreep(pt3, al_map_rgb(255, 255, 255),spd);
	 break;
            
      case 5:
	 pt.y=pt.y+(rand()%200);
	 addCreepB(pt, al_map_rgb(204,3,3), Vector(-60, 0));
	 break;
	 
      case 6:
	 for(int i = 800; i <= 1000; i += 50) {
	    addCreepMis(Point(i,300), Point(700,100), Point(100,100),
			Point(100,500), Point(700,500), al_map_rgb(255,254,253),
			Vector(-90, 0));
	 }
	 break;
      default:
	 break;
   }   
}




void GameHandler::CircleLaser(std::shared_ptr<Enemy> E) {
   
   for(int i = -500; i <= 500; i += 325) {
      for(int j = -500; j <= 500; j += 325) {
	 addLaser(E->getCentre(), E->getColor(), Vector(i, j));
      }
   }
   E->setFire(false);

}
void GameHandler::bossFire(std::shared_ptr<Enemy> e){
   int n = rand() % 7 + 1;
   Point playerloc;
if (spaceShip) {
      playerloc = spaceShip->centre;
   }
   Vector aim(0, 0);
   //change this to be based on lives
   switch(n) {
      case 1:
	 for(int i=-100; i<=100; i+=20)
	    addLaser(e->centre+Point(0, i), e->color, e->getProjSpeed());
	 break;
      case 2:
	 aim.Angle(playerloc, e->centre, 0.9);
	 for(int i = -70; i <= 70; i += 20)
	    addLaser(e->centre+Point(50,0), e->color, aim+ Vector(-30,i));
	break;
      case 3:
	 addCreepB(e->centre+Point(50,0), al_map_rgb(204,3,3), Vector(-100, 0));
	 break;
      default:
	 aim.Angle(playerloc, e->centre+Point(0,50), 0.9);
	 addMissile(e->centre+Point(0,50), al_map_rgb(204, 3, 3), aim);
	 aim.Angle(playerloc, e->centre+Point(0,-50), 0.9);
	 addMissile(e->centre+Point(0,-50), al_map_rgb(204, 3, 3), aim);
	 break;
   }
}
   



//==================================
// UPDATE FUNCTIONS
//=================================
void GameHandler::spawnBoss() {
   addBoss(Point(850, 300), al_map_rgb(155, 0, 0), Vector(-50, 0));
   bossExists = true;

}

void GameHandler::addBoss(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   enem.push_back(std::make_shared<Boss> (cen, col, spd));
}

void GameHandler::updateBoss() {
   // spawn condition
   if (bossSpawnConditionCounter > 30 && bossExists == false) {
      // start timer
      if (bossTimer->isRunning() == false) {
	 bossTimer->startTimer();
	 bossIncoming = true;
      }
      if (bossTimer->getCount() > 250) {
	 spawnBoss(); // bossExists set to true from here
	 bossTimer->stopTimer();
	 bossTimer->resetCount();
	 bossIncoming = false;
      }      
   }
}

void GameHandler::drawBossIncomingMessage() {
   if (bossTimer->isRunning() == true && bossIncoming == true) {
      //gameOverFont->drawTextCentered(al_map_rgb(204, 204, 0), "BOSS INCOMING");
      if (bossTimer->getCount() > 200) {
	 bossIncoming = false;
      }      
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
   if (!enem.empty()) {
      for (auto it = enem.begin(); it != enem.end(); ++it) {
	 (*it)->update(dt);
	 //fire routines for regular enemies	 
	 if ((*it)->getFire()) {
	    //explosion of lasers from creepBomb	    
	    if (doColorsMatch((*it)->color, al_map_rgb(204,3,3))) {
	       CircleLaser((*it));
	    }
	    //boss fire mechanics
	    if (doColorsMatch((*it)->color, al_map_rgb(155, 0, 0))) {
	       bossFire((*it));
	    }	    
	    //Purple enemies will spawn two extra projectiles
	    else if (doColorsMatch((*it)->color, al_map_rgb(246, 64, 234))) {
	       addLaser((*it)->centre, (*it)->color, (*it)->getProjSpeed() + Vector(0, 40));
	       addLaser((*it)->centre, (*it)->color, (*it)->getProjSpeed() + Vector(0, -40));
	    }	    
	    else {
	       addLaser((*it)->centre + Vector(20, 0), (*it)->color, (*it)->getProjSpeed());
	    }
	    (*it)->setFire(false);
	 }
      }
   }

   if (enem.size() <= 3 && bossExists == false && !bossIncoming) {
      spawn();
   }
}

void GameHandler::collision() {
   checkCollisionOnPlayer();
   checkCollisionOnEnemies();
   checkCollidingEnemyWithPlayer();
}

void GameHandler::checkCollisionOnEnemies() {
   /*
   if (!proj.empty() && !enem.empty() && spaceShip) {
      for (std::list< std::shared_ptr<Projectile> >::iterator it_proj = 
	      proj.begin(); it_proj != proj.end(); ++it_proj) {
         if (doColorsMatch(player->color, (*it_proj)->color)) {
            for (std::list< std::shared_ptr<Enemy> >::iterator it_enem = 
               enem.begin(); it_enem != enem.end(); ++it_enem) {
            
               // set bounding points
               Point pt_proj = (*it_proj)->centre;
               Point pt_enem = (*it_enem)->getCentre();
               int enem_size = (*it_enem)->getSize();

               // check for collision
               if ((pt_proj.x > pt_enem.x - enem_size) &&
               (pt_proj.x < pt_enem.x + enem_size) &&
               (pt_proj.y > pt_enem.y - enem_size) &&
               (pt_proj.y < pt_enem.y + enem_size)) {
               
            // register damage on enemy and flag projectile as dead
            (*it_proj)->live = false;
            (*it_enem)->hit();
               }
            }
         }
      }
   }
   */
}

void GameHandler::checkCollidingEnemyWithPlayer() {
   /*
   if (!enem.empty() && spaceShip) {      
      for (auto it = enem.begin(); it != enem.end(); ++it) {	 
	 if ((*it)->dead == false) {	    
	    if (doHitboxesIntersect(spaceShip->centre, 16, (*it)->centre, 
				    (*it)->getSize())) {
	       (*it)->hit();
	       spaceShip->hit(1);
	    }
	 }	       
      }
   } 
   */ 
}


void GameHandler::checkCollisionOnPlayer() {
   /*
   if (!proj.empty() && spaceShip) {
      for (auto it = proj.begin(); it != proj.end(); ++it) {	    
	 if (!doColorsMatch((*it)->color, spaceShip->color)) {
	    if (isPointBoxCollision((*it)->centre, spaceShip->centre, 16)) {	  
	       // register damage on player and flag projectile as dead
	       (*it)->live = false;
	       spaceShip->hit(1);
	    }
	 }
      }
   }
   */
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
   std::list< std::shared_ptr<Enemy> > newEnem;
   if (!enem.empty()) {
      for (auto it = enem.begin(); it != enem.end(); ++it) {
	 // color check for boss
	 if (doColorsMatch((*it)->getColor(), al_map_rgb(155, 0, 0))) {
	    if ((*it)->getdAnim_complete()) {
	       if(lives<3)
		  lives++;
	       bossExists = false;
	       bossFirstShot = false;
	       bossSpawnConditionCounter=0;
	    }
	 }
	 if (!(*it)->getdAnim_complete()) {
	    newEnem.push_back(*it);
	 }
      }
      enem.clear();
      enem.assign(newEnem.begin(), newEnem.end());
   }
}

void GameHandler::addLaser(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   lasersList.push_back(std::make_shared<Laser> (cen, col, spd));
}

void GameHandler::addMissile(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   std::shared_ptr<Missile> missileObject = std::make_shared<Missile> (cen, col, spd);
   missileObject->load_assets();
   missileList.push_back(missileObject);
}

// function to add a Creep object onto the Enemy list
void GameHandler::addCreep(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   enem.push_back(std::make_shared<Creep> (cen, col, spd));
}

// function to add a CreepBomb object onto the Enemy list
void GameHandler::addCreepB(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   enem.push_back(std::make_shared<CreepBomb>(cen, col, spd));
}


void GameHandler::addCreepMis(const Point& cen, Point p1, Point p2, Point p3, Point p4,
			 const ALLEGRO_COLOR& col, const Vector& spd) {
   enem.push_back(std::make_shared<CreepMis>(cen, p1, p2, p3, p4, col, spd));
}


void GameHandler::addPlayerLaserSingleShot() {
   addLaser(spaceShip->centre + Point(-20, 0), spaceShip->color, SPACE_SHIP_PROJECTILE_SPEED);
   laserShotsTimer->srsTimer();
}

void GameHandler::addPlayerMissileSingleShot() {
   addMissile(spaceShip->centre, spaceShip->color, SPACE_SHIP_PROJECTILE_SPEED);
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

