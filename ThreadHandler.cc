#include "ThreadHandler.h"

__BEGIN_API

Thread* ThreadHandler::gameloopThread;
Thread* ThreadHandler::spaceShipThread;
Thread* ThreadHandler::windowThread;
Thread* ThreadHandler::keyboardThread;
Thread* ThreadHandler::enemySpawnThread;
Thread* ThreadHandler::collisionAndLifeThread;
Thread* ThreadHandler::bossThread;
Thread* ThreadHandler::bombHandlerThread;
Thread* ThreadHandler::gameOverThread;

GameLoopHandler* ThreadHandler::gameLoop;
std::shared_ptr<GameHandler> ThreadHandler::gameHandler;
Window* ThreadHandler::window;
Keyboard* ThreadHandler::keyboard;
EnemySpawn* ThreadHandler::enemySpawn;
CollisionAndLife* ThreadHandler::collisionAndLife;
BossHandler* ThreadHandler::bossHandler;
BombHandler* ThreadHandler::bombHandler;
GameOverHandler* ThreadHandler::gameOverHandler;

__END_API