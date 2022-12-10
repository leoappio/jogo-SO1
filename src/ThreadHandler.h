#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include <iostream>
#include "cpu.h"
#include "traits.h"
#include "thread.h"
#include "Engine.h"
#include "GameHandler.h"

__BEGIN_API

class GameHandler;

class ThreadHandler{
    public:
    ThreadHandler(){}
    ~ThreadHandler(){}

    static void execute(void * name){
        engineThread = new Thread(EngineExecutor);
        spaceShipThread = new Thread(SpaceShipExecutor);
        
        
        engineThread->join();
        spaceShipThread->join();
        
        delete engineThread;
        delete spaceShipThread;
    }


    static void EngineExecutor(){
        engine = new Engine(800, 600, 60);
        engine->init();
        engine->run();
    }

    static void SpaceShipExecutor(){
        engine->gameHandler->spaceShip->run();
    }

    static Thread* engineThread;
    static Thread* spaceShipThread;
    //static Thread* keyboardThread;
    //static Thread* windowThread;
    //static Thread* collisionAndLifeThread;
    //static Thread* enemySpaceShipThread;
    //static Thread* bombHandlerThread;
    //static Thread* bossThread;

    static SpaceShip* spaceShip;
    static Engine* engine; 


};

__END_API
#endif