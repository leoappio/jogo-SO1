#ifndef GAMELOOPHANDLER_H
#define GAMELOOPHANDLER_H

#include <allegro5/allegro.h>
#include "traits.h"
#include <memory>

__BEGIN_API

class GameLoopHandler{
    public:
      std::shared_ptr<GameHandler> gameHandler;
      GameLoopHandler(){}
      ~GameLoopHandler(){}
      void gameLoop();
      float prevTime = 0;
};

__END_API
#endif