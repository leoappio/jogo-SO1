#ifndef ACTION_H
#define ACTION_H
#include "traits.h"

__BEGIN_API
namespace act {

   enum action {
      NO_ACTION,
      MOVE_UP,
      MOVE_RIGHT,
      MOVE_DOWN,
      MOVE_LEFT,
      FIRE_PRIMARY,
      FIRE_SECONDARY,
      QUIT_GAME
   };

}
__END_API
#endif
