
#include "Keyboard.h"
#include "traits.h"
#include "GameHandler.h"
#include "thread.h"

__BEGIN_API

void Keyboard::run(){
    while(!gameHandler->gameOver){
        al_get_keyboard_state(&kb);   

        if (al_key_down(&kb, ALLEGRO_KEY_UP)) {
            gameHandler->actionsList.push_back(act::action::MOVE_UP);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_RIGHT)) {
            gameHandler->actionsList.push_back(act::action::MOVE_RIGHT);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_DOWN)) {
            gameHandler->actionsList.push_back(act::action::MOVE_DOWN);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_LEFT)) {
            gameHandler->actionsList.push_back(act::action::MOVE_LEFT);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_SPACE)) {
            gameHandler->actionsList.push_back(act::action::FIRE_PRIMARY);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_1)) {
            gameHandler->actionsList.push_back(act::action::FIRE_SECONDARY);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_ESCAPE)) {
            gameHandler->actionsList.push_back(act::action::QUIT_GAME);
        }
    
        Thread::yield();
    }
}
__END_API
