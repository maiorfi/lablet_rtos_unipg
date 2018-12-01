#include "mbed.h"

#include "SWO.h"

static DigitalOut led1(LED1);
static InterruptIn btn(BUTTON1);

// Diagnostic trace channel implemented at core level (much faster than UART; also I don't want to reserve a UART just for tracing)
SWO_Channel swo("channel");

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    // Initialization and all kind of stuff here...

    swo.printf("[MAIN] ...exiting from main()\n");
}