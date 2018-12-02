## ESERCIZIO SU RTOS

#### Problema:
Realizzare un firmware in cui 2 thread hanno 2 funzioni distinte e rispettivamente dedicate a svolgere, senza soluzione di continuità, le seguenti attività:
+ *(reader thread)* leggere lo stato di un ingresso digitale (in polling, senza l'uso di interrupt) per rilevare la lunghezza (temporale) di impulsi "alti" (ossia compresi tra un fronte di salita e il successivo fronte di discesa). Per ciascun impulso rilevato il thread aggiunge la lunghezza dell'impulso ad una coda globale (definita a livello di applicazione, non di funzione eseguita dal thread) definita come una ```std::deque<int>```
+ *(writer thread)* leggere lo stato della coda con logica FIFO, utilizzando ciascun item contenuto per eseguire un diverso pattern di attivazione di 3 uscite digitali (ad esempio attivare per un tempo costante una delle tre uscite selezionata sulla base del valore dell'item "scodato") 