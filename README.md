## STEP #0

## (ciascun branch contiene un differente esempio)

### La compilazione richiede che vengano prima scaricati i sorgenti della libreria "puntata" da SWO.lib [Pubblicata qui](https://mbed.org/users/wim/code/SWO/) utilizzando il comando __mbed update__ dall'interno della directory di progetto

### Per i sample che utilizzano la Standard Library è necessario che il compilatore utilizzi la versione C++11 (o superiore) della libreria. Per configurare il compilatore GCC in tal senso è necessario modificare i file debug.json, release.json e develop.json contenuti in mbed-os/tools/profiles sostituendo, all'interno dell'array associato alla proprietà "cxx" della sezione "GCC_ARM", la stringa "-std=gnu++98" con "-std=gnu++11" (lo stesso può essere fatto anche per il compilatore ARMC6) 

+ Struttura di base del progetto.
+ Utilizzo della porta diagnostica SWO (Serial Wire Trace Output - [Info](https://www.arm.com/files/pdf/AT_-_Advanced_Debug_of_Cortex-M_Systems.pdf))