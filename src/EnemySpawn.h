#ifndef ENEMYSPAWN_H
#define ENEMYSPAWN_H

#include <allegro5/allegro.h>
#include "traits.h"
#include <memory>

__BEGIN_API

class EnemySpawn{
    public:
      std::shared_ptr<GameHandler> gameHandler;
      EnemySpawn(){}
      ~EnemySpawn(){}
      void run();
};

__END_API
#endif