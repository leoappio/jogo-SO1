#ifndef BOSS_H
#define BOSS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>
#include "Point.h"
#include "Vector.h"

__BEGIN_API

struct Point;
struct Vector;
class Timer;
class Sprite;

extern const int BOSS_HP;

class Boss{

  private:
   std::shared_ptr<Timer> delayTimer;
   
  public:
   Boss (Point p, ALLEGRO_COLOR c, Vector s);
   ~Boss();
   Point centre;
   ALLEGRO_COLOR color;
   Vector speed;
   Vector enemyLaserSpeed;
   bool dead;
   bool dAnim_complete;
   bool fire;
   bool aliveBoss;
   int bossSize;
   int fireSpeed; 
   int lives;
   int dAnim;
   bool targetable;
   int col, row, spriteSheetIndex;

   
   void update(double dt);
   void load_assets();
   void deathAnim(std::shared_ptr<Sprite>);
   void hit();
   void chooseFrame();
   void draw(std::shared_ptr<Sprite> ship, std::shared_ptr<Sprite> death);
};

__END_API
#endif
