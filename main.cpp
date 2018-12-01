#include "mbed.h"

#include "SWO.h"

#include <tuple>

static DigitalOut ledOnBoard(LED1);
static InterruptIn btnOnBoard(BUTTON1);

static DigitalOut ledRed(PD_0, true);
static DigitalOut ledYellow(PD_1, true);
static DigitalOut ledGreen(PG_0, true);

static InterruptIn btnBlack(PA_15);

SWO_Channel swo("channel");

static Timer s_boot_timer;

static Thread s_thread_1;
static Thread s_thread_2;
static Thread s_thread_3;

static void thread_procedure(std::tuple<DigitalOut *, int> *args)
{
    DigitalOut *digitalOut = std::get<0>(*args);
    int delayMs = std::get<1>(*args);

    int state = 0;

    while (true)
    {
        wait_ms(delayMs);
        digitalOut->write(state);

        state = (state ? 0 : 1);
    }
}

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    s_boot_timer.start();

    std::tuple<DigitalOut *, int> args1(&ledGreen, 500);
    std::tuple<DigitalOut *, int> args2(&ledYellow, 200);
    std::tuple<DigitalOut *, int> args3(&ledRed, 300);

    s_thread_1.start(callback(thread_procedure, &args1));
    s_thread_2.start(callback(thread_procedure, &args2));
    s_thread_3.start(callback(thread_procedure, &args3));

    while(true) {}

    swo.printf("[MAIN] ...exiting from main()\n");
}
