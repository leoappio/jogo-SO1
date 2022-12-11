#include "GameOverHandler.h"
#include "traits.h"
#include "GameHandler.h"
#include "thread.h"

__BEGIN_API

void GameOverHandler::run(){
    while(!gameHandler->gameOver){
        if (gameHandler->lives <= 0) {
            gameHandler->bg->draw_parallax_background(gameHandler->bgMid.x, 0);
            ALLEGRO_FONT* font = al_load_font("DavidCLM-BoldItalic.ttf", 64, 0);
            al_draw_text(font, al_map_rgb(204, 3, 3), 400, 300, ALLEGRO_ALIGN_CENTRE, "GAME OVER!");
            gameHandler->enemySpaceShipsList.clear();
            gameHandler->boss = nullptr;
            gameHandler->bombEnemiesList.clear();
            gameHandler->lasersList.clear();
            gameHandler->missileList.clear();
        }

        Thread::yield();
    }
}

__END_API