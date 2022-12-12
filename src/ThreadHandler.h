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
#include "CollisionAndLife.h"
#include "BossHandler.h"
#include "BombHandler.h"
#include "GameOverHandler.h"

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
        collisionAndLifeThread = new Thread(collisionAndLifeExecutor);
        bombHandlerThread = new Thread(bombHandlerExecutor);
        bossThread = new Thread(bossHandlerExecutor);
        gameOverThread = new Thread(gameOverHandlerExecutor);
        
        
        gameloopThread->join();
        windowThread->join();
        keyboardThread->join();
        spaceShipThread->join();
        enemySpawnThread->join();
        collisionAndLifeThread->join();
        bombHandlerThread->join();
        bossThread->join();
        gameOverThread->join();

        
        delete gameloopThread;
        delete windowThread;
        delete keyboardThread;
        delete spaceShipThread;
        delete enemySpawnThread;
        delete collisionAndLifeThread;
        delete bombHandlerThread;
        delete bossThread;
        delete gameOverThread;
        gameHandler.reset();
    }


    static void gameLoopExecutor(){
        gameLoop = new GameLoopHandler();
        gameLoop->gameHandler = gameHandler;
        gameLoop->gameLoop();
        delete gameLoop;
    }

    static void windowExecutor(){
        window = new Window();
        window->gameHandler = gameHandler;
        window->run();
        delete window;
    }

    static void keyboardExecutor(){
        keyboard = new Keyboard();
        keyboard->gameHandler = gameHandler;
        keyboard->run();
        delete keyboard;
    }

    static void enemySpawnExecutor(){
        enemySpawn = new EnemySpawn();
        enemySpawn->gameHandler = gameHandler;
        enemySpawn->run();
        delete enemySpawn;
    }

    static void collisionAndLifeExecutor(){
        collisionAndLife = new CollisionAndLife();
        collisionAndLife->gameHandler = gameHandler;
        collisionAndLife->run();
        delete collisionAndLife;
    }

    static void bossHandlerExecutor(){
        bossHandler = new BossHandler();
        bossHandler->gameHandler = gameHandler;
        bossHandler->run();
        delete bossHandler;
    }

    static void spaceShipExecutor(){
        gameHandler->spaceShip->gameHandler = gameHandler;
        gameHandler->spaceShip->run();
    }

    static void bombHandlerExecutor(){
        bombHandler = new BombHandler();
        bombHandler->gameHandler = gameHandler;
        bombHandler->run();
        delete bombHandler;
    }

    static void gameOverHandlerExecutor(){
        gameOverHandler = new GameOverHandler();
        gameOverHandler->gameHandler = gameHandler;
        gameOverHandler->run();
        delete gameOverHandler;
    }

    static Thread* gameloopThread;
    static Thread* spaceShipThread;
    static Thread* windowThread;
    static Thread* keyboardThread;
    static Thread* enemySpawnThread;
    static Thread* collisionAndLifeThread;
    static Thread* bossThread;
    static Thread* bombHandlerThread;
    static Thread *gameOverThread;

    static GameLoopHandler* gameLoop;
    static Window* window;
    static Keyboard* keyboard;
    static EnemySpawn* enemySpawn;
    static CollisionAndLife* collisionAndLife;
    static BossHandler* bossHandler;
    static BombHandler* bombHandler;
    static GameOverHandler* gameOverHandler;
    static std::shared_ptr<GameHandler> gameHandler;
};

__END_API
#endif