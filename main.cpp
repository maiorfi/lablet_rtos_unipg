#include "mbed.h"

#include "SWO.h"

static DigitalOut led1(LED1);
static DigitalOut led2(LED2);
static DigitalOut led3(LED3);

static DigitalIn btn(USER_BUTTON);

SWO_Channel swo("channel");

static Thread s_thread_1;
static Thread s_thread_2;

static EventFlags s_event_flags;

#define EVENT_FLAG_ID_1 (0x01)

// This is the (never-ending, in this case) function run by "s_thread_1"
static void thread_procedure_1()
{
    while(true)
    {
        if(s_event_flags.get() & EVENT_FLAG_ID_1)
        {
            // just toggle on-board led
            led1.write(!led1.read());

            wait(0.5);
        }
        else
        {
            led1.write(false);
        }
    }
}

static void thread_procedure_2()
{
    while(true)
    {
        if(btn.read())
        {
            s_event_flags.set(EVENT_FLAG_ID_1);
        }
        else
        {
            s_event_flags.clear(EVENT_FLAG_ID_1);
        }
    }
}

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    s_thread_1.start(thread_procedure_1);
    s_thread_2.start(thread_procedure_2);

    // at this point we have 2 threads running: the one running main() and the one running thread_procedure()
    swo.printf("[MAIN] ...exiting from main()\n");
}