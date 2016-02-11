# JASM
JASM, acronimo di **JustAnotherSystemMonitor** è un monitor di sistema in via di
sviluppo. Diversamente da un qualsiasi monitor di sistema JASM è concepito per
essere un tool, atto al monitoraggio delle risorse hardware e software, con il
minimo impatto sulle prestazioni del sistema, **indipendente da una GUI
prestabilita** e sopratutto **facilmente estendibile nel tempo**.

JASM è un processo demone (è prevista la possibilità di scegliere, se è root a
deciderlo, di eseguire il programma al boot del sistema) che gira in backgound
nell'attesa che sia stabilita una connessione su socket (che sia locale o, se
l'utente root decide, remota), una volta connesso un client è possibile
acquisire informazioni di ogni tipo sul sistema operativo e sulla macchina sulla
quale gira JASM. I client che dialogano su socket con il monitor di sistema sono
programmi client GUI, ciò è pensato per lasciare all'utente la libertà di
scegliere l'interfaccia grafica che più gli aggrada, che sia semplicemente una
command line, piuttosto che una GUI in GTK o in Qt o in qualsiasi altro tool
grafico in qualsiasi linguaggio (questo è **software libero**, leggendo il readme
per i programmatori dei client GUI che descrivono le API di JASM e il loro
funzionamento puoi aiutare lo sviluppo e garantirti un posticino del buon caro
paradiso Stallmaniano ;-D).

JASM è scritto in **C11**, sviluppato al momento per OS UNIX-like, ma in un
futuro non molto lontano sarà compilabile anche su Windows.
Il codice è strutturato in maniera tale da poter estendere nel tempo le
funzionalità di JASM evitando di diventare pazzi leggendo ogni riga di codice
del programma, scrivendo, invece, librerie e header che astraggono interfacce
standard stabilite nei futuri readme (nel frattempo inizia a leggerti il codice)
e inserendo il nome dell'header in un array di stringhe in "modules.c".
Disponibile anche la possibilità di estendere JASM programmando singole funzioni
che inviano ai client delle brevi info, quali ad esempio il tipo di architettura,
il nome dell'host o la versione del sistema operativo; per maggiori info aspetta
i futuri readme o vai a leggere "getter.c".

Stay tooned e aiuta la causa: code review, suggerimenti, idee e programmatori
sono essenziali per l'avanzamento del progetto. Per qualsiasi domanda invia una
mail ad alessandrospallina1@gmail.com, prometto risposte instantanee :D
