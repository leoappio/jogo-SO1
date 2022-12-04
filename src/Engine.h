/**
 * @file Engine.h
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <allegro5/allegro.h>
#include <memory>
#include <string>

class GameHandler;

class Engine {
  public:
   Engine(int w, int h, int fps);
   ~Engine();
   
   void init();
   void run();
   void draw();
   void update(double dt);
   void gameLoop(float& prevTime);

   inline int getWidth() const {
      return _displayWidth;
   }
   inline int getHeight() const {
      return _displayHeight;
   }
   inline int getFps() const {
      return _fps;
   }

  private:
   int _displayWidth;
   int _displayHeight;
   int _fps;

   // allegro objects
   ALLEGRO_TIMER *_timer;
   ALLEGRO_EVENT_QUEUE *_eventQueue;
   ALLEGRO_DISPLAY *_display;

   bool _finish;

   // class to handle with all game components
   std::shared_ptr<GameHandler> _gameHandler; 
};

#endif