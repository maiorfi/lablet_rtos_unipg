#include "mbed.h"

#include "SWO.h"

#include <deque>

static DigitalOut ledOnBoard(LED1);
static InterruptIn btnOnBoard(BUTTON1);

SWO_Channel swo("channel");

static Thread thread_blink;                            // per il task che fa lampeggiare il led
static Thread thread_periodic;                        // per il task periodico
static Thread thread_button_queue_handler;             // per il task che esegue la callback creata nella coda dal gestore di interrupt del pulsante

static EventQueue eq_periodic;                        // coda che mantiene le operazioni (chiamate a funzioni) del task periodico
static EventQueue eq_button_interrupt;                 // coda che mantiene le operazioni (chiamate a funzioni, o callback) generate dal gestore di interrupt del pulsante

static Timer timer;                                    // cronometro che misura il tempo trascorso dal boot

// task (avviato nel main) eseguito periodicamente
// nota: il parametro "c" viene passato dal main(); questo è un modo per avviare più task che eseguono indipendentemente la stessa funzione ma con contesto diverso
void event_function_periodic(char c)
{
    // messaggio di debug; legge il valore del timer/cronometro
    swo.printf("[PERIODIC - event_function_periodic] Trascorsi %d ms (%c)\n", timer.read_ms(), c);
}

// task (avviato nel main) che in un ciclo infinito effettua il toggling della porta di uscita collegata al led e attende (senza bloccare gli altri task) mezzo secondo
void thread_function_blink(DigitalOut* pled)
{
    while (true)
    {
        pled->write(!pled->read());
        wait_ms(500);
    }
}

void event_interrupt_handler(bool state)
{
    swo.printf("[EVENT-HANDLER - btn_interrupt_handler] Interrupt pulsante: %s\n", state ? "L->H" : "H->L");
}

void btn_interrupt_handler()
{
    // Le funzioni di standard I/O della libreria standard del C non sono "interrupt-safe" (né thread-safe, peraltro), quindi la chiamata a printf() che segue NON FUNZIONA!
    // In particolare il firmware genera un errore bloccante dettagliato nella UART di debug (default 8-N-1, 9600 baud) come "Error - writing to a file in an ISR or critical section"
    //swo.printf("[ISR - btn_interrupt_handler] Interrupt pulsante: %s\n", btnOnBoard.read() ? "L->H" : "H->L");

    // Questo invece lo posso fare, perché l'accodamento in una EventQueue è interrupt-safe (e pure thread-safe, peraltro)
    eq_button_interrupt.call(&event_interrupt_handler, btnOnBoard.read());

    // Da provare anche (esecuzione differita di 500ms):
    //eq_button_interrupt.call_in(500, &event_interrupt_handler, btnOnBoard.read());
}

int main()
{
    swo.printf("[MAIN] Starting main()...\n");

    eq_periodic.call_every(5000, &event_function_periodic, '*');                                       // il task periodico viene "programmato" per essere eseguito una volta ogni 5 secondi
    eq_periodic.call_every(3000, &event_function_periodic, '^');                                        // il task periodico viene "programmato" per essere eseguito una volta ogni 3 secondi
    
    thread_blink.start(callback(&thread_function_blink, &ledOnBoard));                                   // il task che effettua il toggling del led viene avviato qui, passando come parametro il puntatore alla porta di uscita da usare

    btnOnBoard.rise(&btn_interrupt_handler);                                                             // registrazione handler (funzione di gestione, detta anche isr) L->H
    btnOnBoard.fall(&btn_interrupt_handler);                                                             // registrazione handler (funzione di gestione, detta anche isr) H->L (stessa dell'interrupt "rise")
    
    timer.start();                                                                                      // il cronometro viene avviato qui
    
    thread_periodic.start(callback(&eq_periodic, &EventQueue::dispatch_forever));                     // il task periodico viene avviato qui
    thread_button_queue_handler.start(callback(&eq_button_interrupt, &EventQueue::dispatch_forever));   // il task che esegue lo scodamento ed esecuzione delle callback create dall'interrupt viene avviato qui

    swo.printf("[MAIN] ...exiting from main()\n");
}
