#include "mbed.h"

#include "SWO.h"

#include <tuple>

// let's use 3 leds to detect threads activity
static DigitalOut ledRed(PD_0, true);
static DigitalOut ledYellow(PD_1, true);
static DigitalOut ledGreen(PG_0, true);

// we're not going to use this right now
static InterruptIn btnBlack(PA_15);

SWO_Channel swo("channel");

// here we have 3 Thread instances
static Thread s_thread_1;
static Thread s_thread_2;
static Thread s_thread_3;

// in this case, the same function is run by all three threads we're going to start
// note:    a std::tuple is a (template) class defined in standard c++ library
//          its purpose is that of containing ordered objects of different types
static void thread_procedure(std::tuple<DigitalOut *, int> *args)
{
    // extract first tuple variable (0-based)
    DigitalOut *digitalOut = std::get<0>(*args);
    
    // extract second tuple variable
    int delayMs = std::get<1>(*args);

    while (true)
    {
        wait_ms(delayMs);

        // just toggle led
        digitalOut->write(!digitalOut->read());
    }
}

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    // a different argument-set passed to each of 3 threads (3 leds, with 3 different toggling interval)
    std::tuple<DigitalOut *, int> args1(&ledGreen, 500);
    std::tuple<DigitalOut *, int> args2(&ledYellow, 200);
    std::tuple<DigitalOut *, int> args3(&ledRed, 300);

    // let's start the threads
    s_thread_1.start(callback(thread_procedure, &args1));
    s_thread_2.start(callback(thread_procedure, &args2));
    s_thread_3.start(callback(thread_procedure, &args3));

    // wait here forever (BEWARE: this is going to "burn" CPU cycles for no reason!)
    while(true) {}

    swo.printf("[MAIN] ...exiting from main()\n");
}
