#include "ThreadHandler.h"

__BEGIN_API

Thread* ThreadHandler::gameloopThread;
Thread* ThreadHandler::spaceShipThread;
Thread* ThreadHandler::windowThread;

GameLoopHandler* ThreadHandler::gameLoop;
std::shared_ptr<GameHandler> ThreadHandler::gameHandler;
Window* ThreadHandler::window;

__END_API