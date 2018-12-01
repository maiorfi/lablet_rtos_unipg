#include "mbed.h"

#include "SWO.h"

#include <deque>

static DigitalOut ledOnBoard(LED1);
static InterruptIn btnOnBoard(BUTTON1);

static DigitalOut ledRed(PD_0);
static DigitalOut ledYellow(PD_1);
static DigitalOut ledGreen(PG_0);

static DigitalIn btnBlack(PF_2);

SWO_Channel swo("channel");

static Thread s_thread_reader;
static Thread s_thread_writer;

static Timer s_main_timer;

static std::deque<int> s_intervals_queue;
static Mutex s_intervals_queue_lock;

static void reader_thread_procedure()
{
    s_main_timer.start();

    bool latest_input_state = !btnBlack.read();

    int latest_rising_timestamp;

    while (true)
    {
        bool current_input_state = !btnBlack.read();

        if (current_input_state != latest_input_state)
        {
            latest_input_state = current_input_state;

            if (current_input_state)
            {
                latest_rising_timestamp = s_main_timer.read_ms();
            }
            else
            {
                int interval = s_main_timer.read_ms() - latest_rising_timestamp;

                if (interval < 10)
                    continue;

                swo.printf("[READER] Pulse: %d ms\n", interval);

                if (interval > 3000)
                {
                    return;
                }
                else
                {
                    s_intervals_queue_lock.lock();

                    s_intervals_queue.push_back(interval);
                    swo.printf("[READER] now there are %d item%s in queue\n", s_intervals_queue.size(), (s_intervals_queue.size() > 1) ? "s" : "");

                    s_intervals_queue_lock.unlock();
                }
            }
        }
    }
}

static void writer_thread_procedure()
{
    int item;

    while (true)
    {
        item=-1;

        s_intervals_queue_lock.lock();

        if (!s_intervals_queue.empty())
        {
            item = s_intervals_queue.front();
            s_intervals_queue.pop_front();
        }

        s_intervals_queue_lock.unlock();

        if (item!=-1)
        {
            swo.printf("[WRITER] dequeued item %d\n", item);
            
            if (item < 500)
            {
                ledGreen.write(true);
                wait_ms(2000);
                ledGreen.write(false);
                wait_ms(500);
            }
            else if (item < 1000)
            {
                ledYellow.write(true);
                wait_ms(2000);
                ledYellow.write(false);
                wait_ms(500);
            }
            else if (item < 1500)
            {
                ledRed.write(true);
                wait_ms(2000);
                ledRed.write(false);
                wait_ms(500);
            }
        }
    }
}

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    s_thread_reader.start(reader_thread_procedure);
    s_thread_writer.start(writer_thread_procedure);

    s_thread_reader.join();

    swo.printf("[MAIN] ...exiting from main()\n");
}
