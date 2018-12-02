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
    s_main_timer.start();

    bool latest_input_state = !btnBlack.read();

    int latest_rising_timestamp;

    while (true)
    {
        // c'è stata una variazione di stato nell'ingresso?
        bool current_input_state = !btnBlack.read();

        if (current_input_state != latest_input_state)
        {
            latest_input_state = current_input_state;

            // è un fronte di salita?
            if (current_input_state)
            {
                // ...memorizzo il timestamp del fronte
                latest_rising_timestamp = s_main_timer.read_ms();
            }
            else
            {
                // ...è un fronte di discesa, calcolo la lunghezza dell'intervallo
                int interval = s_main_timer.read_ms() - latest_rising_timestamp;

                // quelli troppo brevi li scarto (glitch?)
                if (interval < 10)
                    continue;

                // intervallo di più di 3 secondi fa uscire dalla funzione
                if (interval > 3000)
                {
                    return;
                }
                
                s_intervals_queue_lock.lock();

                // aggiungo l'item alla coda
                s_intervals_queue.push_back(interval);
                int size=s_intervals_queue.size();
                swo.printf("[READER] added item %d (%d item%s in queue)\n", interval, size, size > 1 ? "s" : "");

                s_intervals_queue_lock.unlock();
            }
        }
    }
}

// WRITER : scoda gli item (FIFO) e attiva le uscite di conseguenza, "eseguendo" un pattern applicativo
static void writer_thread_procedure()
{
    int item;
    int size;

    while (true)
    {
        item=-1;

        // TODO : proteggere la coda dall'accesso concorrente
        // ???

        // memorizzo su variabili locali i dati della coda che mi interessano
        // nota: questo "caching" mi consente di tenere la coda lockata il meno possibile, massimizzando l'effettivo parallelismo
        if (!s_intervals_queue.empty())
        {
            item = s_intervals_queue.front();
            s_intervals_queue.pop_front();
            size=s_intervals_queue.size();
        }

        // TODO : uscire dalla zona di protezione dall'accesso concorrente della coda
        // ???

        // c'è almeno un elemento nella coda?
        if (item!=-1)
        {
            swo.printf("[WRITER] dequeued item %d (%d in queue)\n", item, size);
            
            // TODO : decodificare l'item (lunghezza intervallo->selezione uscita da attivare)
            // ...in modo che:
            // 1 - se l'impulso dura meno di mezzo secondo va attivato il led verde
            // 2 - se l'impulso dura tra mezzo secondo ed 1 secondo va attivato il led giallo
            // 3 - se l'impulso dura tra un secondo e un secondo e mezzo va attivato il led rosso
            // Nota: le attivazioni devono avere durata fissa (es. 2 secondi) ed essere seguite da una
            //       pausa (es. mezzo secondo) per poter rilevare visivamente il distacco tra attivazioni
            //       successive dello stesso led 

            // ???
        }
    }
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
