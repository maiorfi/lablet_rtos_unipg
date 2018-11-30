#include "mbed.h"

#include "SWO.h"

static DigitalOut ledOnBoard(LED1);
static InterruptIn btnOnBoard(BUTTON1);

SWO_Channel swo("channel");

static Timer s_boot_timer;

static Thread s_thread_1;

static void thread_procedure()
{
    while(true)
    {
        wait(0.5);
        ledOnBoard.write(!ledOnBoard.read());
    }
}

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    s_boot_timer.start();

    s_thread_1.start(thread_procedure);

    swo.printf("[MAIN] ...exiting from main()\n");
}