#include <ctime>
#include "system.h"
#include "Engine.h"
#include "ThreadHandler.h"

__USING_API
int main() {
   srand(time(0)); 
   System::init(&ThreadHandler::execute);
   return 0;
}