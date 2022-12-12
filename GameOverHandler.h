#ifndef GAMEOVERHANDLER_H
#define GAMEOVERHANDLER_H

#include "traits.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>

__BEGIN_API

class GameOverHandler{
   
  public:
   GameOverHandler(){};
   ~GameOverHandler(){};
  
   std::shared_ptr<GameHandler> gameHandler;
   void run();
};

__END_API
#endif