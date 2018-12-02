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

// Coda condivisa tra thread reader e writer
static std::deque<int> s_intervals_queue;

// Mutex per proteggere dall'accesso concorrente (potenzialmente distruttivo!) 
static Mutex s_intervals_queue_lock;

// READER : fa polling sullo stato di un ingresso e accoda le durate degli intervalli di attivazione
static void reader_thread_procedure()
{       
}

// WRITER : scoda gli item (FIFO) e attiva le uscite di conseguenza, "eseguendo" un pattern applicativo
static void writer_thread_procedure()
{
}

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    s_thread_reader.start(reader_thread_procedure);
    s_thread_writer.start(writer_thread_procedure);

    // attendo l'uscita del thread "reader"
    s_thread_reader.join();

    swo.printf("[MAIN] ...exiting from main()\n");
}
