#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "traits.h"
#include <memory>
#include "GameHandler.h"

__BEGIN_API

class Keyboard{
    public:
     Keyboard(){}
     ~Keyboard(){}
     std::shared_ptr<GameHandler> gameHandler;
     ALLEGRO_KEYBOARD_STATE kb;

     void run();


};
__END_API
#endif