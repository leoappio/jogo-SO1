#include "GameLoopHandler.h"
#include "GameHandler.h"
#include "thread.h"

__BEGIN_API

void GameLoopHandler::gameLoop() {
    while(!gameHandler->gameOver){
        ALLEGRO_EVENT event;
        bool redraw = true;
        float crtTime;
        
        // get event
        al_wait_for_event(gameHandler->_eventQueue, &event);
        
        // _display closes
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            gameHandler->gameOver = true;
            return;
        }
        
        // timer
        if (event.type == ALLEGRO_EVENT_TIMER) {
            crtTime = al_current_time();
            gameHandler->isToUpdate = true;
            gameHandler->dtToUpdate = (crtTime - prevTime);
            prevTime = crtTime;
            redraw = true;
        }
        
        // render
        if (redraw && al_is_event_queue_empty(gameHandler->_eventQueue)) {
            redraw = false;      
            gameHandler->isToDraw = true;
            al_flip_display();
        }

        Thread::yield();
    }
}
__END_API
