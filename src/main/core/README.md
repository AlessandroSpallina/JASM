# Core

## Requisiti
* Cross Platform [usare compilazioni condizionali: direttive preproc]
* Non fare assunzioni sull'ambiente grafico, multi GUI tool:
  astrazione di codice sorgente. [IMPORTANTE]:
    * JASM accetta opzioni per definire la GUI, se -gui:cli, ad esempio,
      tutto ciò che è passato come lista di stringhe viene printeffato e quindi
      deallocato. (modulo codice cli.c cli.h)

## TODO
* Accetta comandi base + altri contenuti in plugin (pastebin, export info to,
  remote controller, etc): legge coda di messaggi che contiene comandi.
  COMANDI BASE: (inseriti su coda ed eseguiti eliminando la stringa corrisp.)
              * getcommand: ritorna la lista di comandi
              * startmodule <name>: esegue modulo
              * stopmodule <name>: ferma modulo


* Esegue il codice di ogni modulo su un thread

## File:
* fifo: gestione coda di stringhe [coda di comandi per il core]
