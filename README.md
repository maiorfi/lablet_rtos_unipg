## STEP #7

Questo sample illustra l'utilizzo dell'oggetto [EventQueue](https://os.mbed.com/docs/latest/apis/eventqueue.html) come strumento per attivare l'esecuzione di attività da parte di thread secondari dall'interno di un handler ISR (interrupt service routine). Questa soluzione si rende indispensabile ogni qual volta è necessario svolgere attività non consentite all'interno di un handler ISR, come ad esempio utilizzare la Standard Library del C++.

__Argomenti illustrati:__


+ Utilizzo di EventQueue per schedulazione di operazioni ricorrenti o attivate da ISR