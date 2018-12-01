#include "mbed.h"

#include "SWO.h"

static DigitalOut led1(LED1);
static InterruptIn btn(BUTTON1);

SWO_Channel swo("channel");

// Timer objects are used in order to have a hi-res (us), relative time reference
static Timer s_boot_timer;

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    // Timer is reset and still after having been created, so we have to start it
    s_boot_timer.start();

    while(true)
    {
        // wait for 1 second
        wait(1.0);

        // print out elapsed msecs since program started
        swo.printf("[MAIN] %d msecs elapsed since bootstrap\n", s_boot_timer.read_ms());
    }

    // ...we'll never get here :)
    swo.printf("[MAIN] ...exiting from main()\n");
}