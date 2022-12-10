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
#include "Keyboard.h"
#include "EnemySpawn.h"

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
        keyboardThread = new Thread(keyboardExecutor);
        spaceShipThread = new Thread(spaceShipExecutor);
        enemySpawnThread = new Thread(enemySpawnExecutor);
        
        
        gameloopThread->join();
        windowThread->join();
        keyboardThread->join();
        spaceShipThread->join();
        enemySpawnThread->join();
        
        delete gameloopThread;
        delete windowThread;
        delete keyboardThread;
        delete spaceShipThread;
        delete enemySpawnThread;
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

    static void keyboardExecutor(){
        keyboard = new Keyboard();
        keyboard->gameHandler = gameHandler;
        keyboard->run();
    }

    static void enemySpawnExecutor(){
        enemySpawn = new EnemySpawn();
        enemySpawn->gameHandler = gameHandler;
        enemySpawn->run();
    }


    static void spaceShipExecutor(){
        gameHandler->spaceShip->gameHandler = gameHandler;
        gameHandler->spaceShip->run();
    }

    static Thread* gameloopThread;
    static Thread* spaceShipThread;
    static Thread* windowThread;
    static Thread* keyboardThread;
    static Thread* enemySpawnThread;

    //static Thread* collisionAndLifeThread;
    //static Thread* bombHandlerThread;
    //static Thread* bossThread;

    static GameLoopHandler* gameLoop;
    static Window* window;
    static Keyboard* keyboard;
    static EnemySpawn* enemySpawn;
    static std::shared_ptr<GameHandler> gameHandler;
};

__END_API
#endif