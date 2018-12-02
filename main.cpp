#include "mbed.h"

#include "SWO.h"

SWO_Channel swo("channel");

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    // Initialization and all kind of stuff here...

    swo.printf("[MAIN] ...exiting from main()\n");
}