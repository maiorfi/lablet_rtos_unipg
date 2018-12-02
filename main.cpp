#include "mbed.h"

#include "SWO.h"

#include <cstdlib>
#include <ctime>

// Diagnostic trace channel implemented at core level (much faster than UART; also I don't want to reserve a UART just for tracing)
SWO_Channel swo("channel");

static Thread s_thread_1;
static Thread s_thread_2;
static Thread s_thread_3;

static int s_account_1=1000000;
static int s_account_2=1000000;

static Mutex s_lock;

static void thread_procedure()
{
    while (true)
    {
        // get a random amount
        int amount=100.0 + 900.0 * (((double)std::rand()) / RAND_MAX);
        
        // and a random direction 1->2 or 2->1
        bool direction=(std::rand() % 2 == 0);

        //s_lock.lock();

        if(direction)
        {
            s_account_1+=amount;    // credit
            s_account_2-=amount;    // debit
        }
        else
        {
            s_account_1-=amount;    // debit
            s_account_2+=amount;    // credit
        }

        //s_lock.unlock();
    }
}

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    std::srand(std::time(nullptr)); //use current time as seed for random generator

    s_thread_1.start(thread_procedure);
    //s_thread_2.start(thread_procedure);
    //s_thread_3.start(thread_procedure);

    while(true)
    {
        // is sum of amounts constant as it should be?
        swo.printf("A1: %d A2: %d - A1+A2: %d\n",s_account_1, s_account_2, s_account_1+s_account_2);
        wait_ms(1000);
    }

    swo.printf("[MAIN] ...exiting from main()\n");
}
