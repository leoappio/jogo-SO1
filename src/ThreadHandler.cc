#include "ThreadHandler.h"

__BEGIN_API

Thread* ThreadHandler::gameloopThread;
Thread* ThreadHandler::spaceShipThread;
Thread* ThreadHandler::windowThread;
Thread* ThreadHandler::keyboardThread;

GameLoopHandler* ThreadHandler::gameLoop;
std::shared_ptr<GameHandler> ThreadHandler::gameHandler;
Window* ThreadHandler::window;
Keyboard* ThreadHandler::keyboard;

__END_API