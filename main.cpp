#include "mbed.h"

#include "SWO.h"

static DigitalOut ledOnBoard(LED1);

SWO_Channel swo("channel");

static Timer s_boot_timer;

// A Thread object. It will be started later, in main()
static Thread s_thread_1;

// This is the (never-ending, in this case) function run by "s_thread_1"
static void thread_procedure()
{
    while(true)
    {
        wait(0.5);

        // just toggle on-board led
        ledOnBoard.write(!ledOnBoard.read());
    }
}

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    s_boot_timer.start();

    // let's start a thread, passing function to be executed
    s_thread_1.start(thread_procedure);

    // at this point we have 2 threads running: the one running main() and the one running thread_procedure()
    swo.printf("[MAIN] ...exiting from main()\n");
}