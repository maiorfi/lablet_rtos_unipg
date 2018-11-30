#include "mbed.h"

#include "SWO.h"

static DigitalOut led1(LED1);
static InterruptIn btn(BUTTON1);

SWO_Channel swo("channel");

static Timer s_boot_timer;

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    s_boot_timer.start();

    while(true)
    {
        wait(1.0);
        swo.printf("[MAIN] %d msecs elapsed since bootstrap\n", s_boot_timer.read_ms());
    }

    swo.printf("[MAIN] ...exiting from main()\n");
}