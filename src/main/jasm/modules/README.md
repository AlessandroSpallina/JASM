# Moduli JASM
NOTA: Questo readme è solo una bozza al momento.

## Come funzionano i moduli? [Per i programmatori dei client]
Per non confondere l'output di JASM su socket verso i vari client,
è consigliabile al client l'apertura di un nuovo socket e una volta connesso
richiedere a JASM l'esecuzione su nuovo thread del modulo desiderato. 
L'utente del client chiaramente non avrà percezione di nulla.

## Note implementative
Client apre nuovo socket -> start* -> JASM crea thread, aggiorna la lista dei
moduli in esecuzione ed esegue la funzione mainmodule

## Cosa deve astrarre un modulo?
* void init_module (int fd)
* void start_module (int sec) //secondi di aggiornamento
Funzioni static a piacere.
Compito di JASM è gestire i thread, invisibili ai programmatori dei moduli.

## Cosa accetta come comandi JASM dai client?
* start* -> inizializza l'ambiente per il modulo e lo starta su thread.
* stop* -> stoppa il modulo: stop al thread (che gira in kenel level) e lo inserisce
	   tra i thread riutilizzabili.
