#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include <iostream>
#include <memory>
#include "cpu.h"
#include "traits.h"
#include "thread.h"
#include "GameHandler.h"
#include "GameLoopHandler.h"
#include "Window.h"

__BEGIN_API

class GameHandler;

class ThreadHandler{
    public:
    ThreadHandler(){}
    ~ThreadHandler(){}

    static void execute(void * name){
        
        gameHandler = std::make_shared<GameHandler>();
        gameHandler->init();

        gameloopThread = new Thread(gameLoopExecutor);
        windowThread = new Thread(windowExecutor);
        spaceShipThread = new Thread(SpaceShipExecutor);
        
        
        gameloopThread->join();
        windowThread->join();
        spaceShipThread->join();
        
        delete gameloopThread;
        delete windowThread;
        delete spaceShipThread;
    }


    static void gameLoopExecutor(){
        gameLoop = new GameLoopHandler();
        gameLoop->gameHandler = gameHandler;
        gameLoop->gameLoop();
    }

    static void windowExecutor(){
        window = new Window();
        window->gameHandler = gameHandler;
        window->run();
    }


    static void SpaceShipExecutor(){
        gameHandler->spaceShip->run();
    }

    static Thread* gameloopThread;
    static Thread* spaceShipThread;
    static Thread* windowThread;
    //static Thread* keyboardThread;
    //static Thread* collisionAndLifeThread;
    //static Thread* enemySpaceShipThread;
    //static Thread* bombHandlerThread;
    //static Thread* bossThread;

    static GameLoopHandler* gameLoop;
    static Window* window;
    static std::shared_ptr<GameHandler> gameHandler;



};

__END_API
#endif