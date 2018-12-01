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

EventFlags completionFlags;

#define THREAD_1_COMPLETED (1 << 0)
#define THREAD_2_COMPLETED (1 << 1)
#define THREAD_3_COMPLETED (1 << 2)

using MyThreadsArguments = std::tuple<int, DigitalOut *, int, int, uint8_t>;

static void thread_procedure(MyThreadsArguments *args)
{
    int threadId = std::get<0>(*args);
    DigitalOut *digitalOut = std::get<1>(*args);
    int delayMs = std::get<2>(*args);
    int iterations = std::get<3>(*args);
    uint8_t completionFlag = std::get<4>(*args);

    swo.printf("[Thread #%d] Start...\n", threadId);

    for (auto i = 0; i < iterations; i++)
    {
        wait_ms(delayMs);
        digitalOut->write(!digitalOut->read());
    }

    digitalOut->write(false);

    swo.printf("[Thread #%d] ...done.\n", threadId);

    completionFlags.set(completionFlag);
}

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    s_boot_timer.start();

    MyThreadsArguments args1(1, &ledGreen, 500, 10, THREAD_1_COMPLETED);
    MyThreadsArguments args2(2, &ledYellow, 200, 20, THREAD_2_COMPLETED);
    MyThreadsArguments args3(3, &ledRed, 300, 30, THREAD_3_COMPLETED);

    s_thread_1.start(callback(thread_procedure, &args1));
    s_thread_2.start(callback(thread_procedure, &args2));
    s_thread_3.start(callback(thread_procedure, &args3));

    completionFlags.wait_all(THREAD_1_COMPLETED | THREAD_2_COMPLETED | THREAD_3_COMPLETED);

    swo.printf("[MAIN] ...exiting from main()\n");
}
