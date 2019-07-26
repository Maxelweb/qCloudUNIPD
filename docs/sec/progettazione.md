
Progettazione
=============

L'approccio di sviluppo per il progetto si è basato sul pattern
**Model-View** di *Qt*. Oltre alla gerarchia e al gestore XML
input/output, sono stati realizzati degli adattatori e un filtro di
ricerca che comunicano alla view i cambiamenti del modello. In questa
sezione verranno analizzati gli elementi principali del modello e della
view, facendo chiarezza sulle opportune dipendenze.

![gerarchia](images/gerarchia)

Modello e Gerarchia
-------------------

### Gerarchia principale

La gerarchia principale del modello si costituisce di una *base
astratta* `Server` da cui derivano direttamente 3 classi concrete:
`VPS`, `Dedicated` e `Hosting`.

Ciascuna classe implementa dei metodi virtuali puri che riguardano la
**serializzazione e deserializzazione dei dati**, **i limiti di cores,
RAM e disco**, **la clonazione profonda**, **il calcolo della velocità
della CPU**, **l'obbligo di virtualizzazione**, **il peso delle
risorse** e **il calcolo del prezzo**.

### La classe Node

La classe `Node` implementa un tipo molto basilare che viene utilizzato
nel modello e che caratterizza un singolo nodo hardware di cui fanno
parte dei server. Questa classe è pensata per rimanere indipendente,
sebbene faccia uso della stessa deserializzazione della gerarchia e sia
compatibile con il `DeepPtr`.

### Il template ausiliario della gerarchia

Al fine di semplificare ed aumentare l'estendibilità della gerarchia si
è optato di tenere traccia dei tipi delle classi concrete attraverso
l'implementazione di una **mappa statica**. Essa contiene come
**chiave** una stringa che è il nome della classe e come **valore** un
oggetto dello stesso tipo.

Inoltre, la mappa viene utilizzata per la parte di serializzazione e
deserializzazione dei dati e utilizza il polimorfismo per richiamare i
metodi virtuali. La costruzione della mappa è la seguente:

-   La classe base `server` ha una mappa statica come membro protetto.

-   Ciascuna classe derivata ha un campo dati statico di tipo
    `StaticTypeBuilder<T>`, dove `T` identifica il nome della classe da
    cui viene richiamato.

-   All'avvio del programma, vengono costruiti i campi dati statici e la
    mappa viene popolata per mezzo del template che esegue esattamente
    questa funzione ausiliaria per la gerarchia.

In questo modo, per estendere ulteriormente la gerarchia, garantendo il
funzionamento autonomo della gestione dati, è sufficiente *dichiarare il
campo dati statico e istanziarlo* con il costruttore di default,
agevolando così la *conoscenza* da parte del builder di un nuovo tipo
concreto. Nel caso in cui la nuova classe fosse astratta (e derivata
dalla base) si potrà omettere semplicemente tale implementazione.
Ovviamente i relativi metodi virtuali puri alla base dovranno essere
implementati per garantirne pieno funzionamento.

### Il container e il deep pointer

Il container realizzato per questo progetto è una lista concatenata
singolarmente con le principali funzioni di *inserimento, rimozione,
ricerca, swap, clonazione profonda e distruzione profonda*. Viene fatto
uso di un contatore degli oggetti all'interno della lista e anche di due
puntatori all'inizio e alla fine della lista. Il container, inoltre, fa
uso di un iteratore costante, mentre per l'iterazione non costante viene
usato direttamente l'operatore di subscripting (`operator[]`) e la
funzione `Select(int i)`, in accoppiata al metodo ausiliario privato
`Jump(unsigned int)`.

In aiuto alla gerarchia, infine, si fa uso del **Deep Pointer**
implementato tramite template in maniera indipendente che semplifica le
operazioni di eliminazione e di clonazione degli oggetti della gerarchia
inseriti all'interno del container. In aggiunta, il `DeepPtr<T>` è
compatibile anche con la classe `Model` e `Node` per garantire
distruzione profonda degli oggetti, dove richiesto.

### La classe Model

Il modello (`Model`) si compone di tutte le classi precedentemente
menzionate e principalmente fa uso di due container di oggetti
`DeepPtr<Node>` e `DeepPtr<Server>`. Sulla base di questi contenitori,
vengono create anche delle **mappe ausiliarie** basate sui nodi che sono
usate per ricavare *dati statistici e avvisi*.

Il modello si occupa anche della gestione dei dati tramite file XML,
controllandone le funzioni di creazione, salvataggio, caricamento ed
esportazione, di cui verrà menzionato meglio il funzionamento in
seguito.\
Questa classe, inoltre, si occupa anche di **aggiungere ed eliminare
oggetti** nei contenitori principali, gestendo gli esiti di ciascuna
operazione.

### Adattatori delle tabelle e filtri di ricerca

Al fine di far comunicare direttamente il *modello* con la *view*, sono
stati creati due *adattatori* derivati dalla classe
`QAbstractTableModel` e un *filtro* utilizzato per la ricerca dei server
che si interpone tra l'adattatore e la view della tabella dei server,
con classe derivata da `QSortFilterProxyModel`.

Entrambi gli adattatori possiedono un *puntatore semplice al modello*
che fa uso dei suoi metodi in base alle necessità. Le funzioni
implementate per le tabelle permettono la visualizzazione, l'aggiunta e
l'eliminazione di righe nella tabella. Al fine di semplificare
l'usabilità, la modifica delle entry è disponibile solamente tramite
l'apposito modulo *Modifica* che compare a destra a fianco della tabella
e non tramite modifica alle singole celle.

I filtri di ricerca permettono la ricerca testuale su 4 campi distinti
del tipo `Server` e, separatamente tramite menù a tendina, sul nodo di
appartenenza. In questo modo è possibile visualizzare tutti i server di
un singolo nodo ed eseguire delle ricerche testuali più approfondite.

Graphical User Interface (GUI)
------------------------------

### La splash window

Eseguendo il programma, viene visualizzata una *splash window* durante
il caricamento delle impostazioni, ossia una piccola finestra a tempo
che mostra il logo del programma e le informazioni di caricamento. La
classe `SplashWindow` deriva direttamente da `QSplashScreen` e
inizializza il banner insieme allo stylesheet (CSS) dell'applicazione.
La classe prende per riferimento un oggetto `MainWindow` che viene poi
*visualizzato* alla fine del caricamento nel metodo `execute`.

### La finestra principale

La finestra principale (`MainWindow`) è una classe derivante da
`QWidget` ed è la classe che gestisce tutta la schermata principale
della GUI, i cui membri privati sono principalmente puntatori a oggetti
di tipo `QWidget`, `QLayout` e, ovviamente, un puntatore profondo al
modello. Questa implementazione permette l'aggiornamento degli elementi
visualizzati solo strettamente necessari e stabilisce una comunicazione
con il modello per le principali richieste di informazioni o di
aggiornamento dei due contenitori.\
Nota particolare è il metodo statico `cleanLayout(QLayout *)` che
permette la cancellazione ricorsiva dei widget e dei layout figli,
garantendone la cancellazione prima di un eventuale refresh.

La finestra principale, infine, fa uso degli oggetti di due classi
ausiliarie (`QLabelTime` e `QChungusBar`) per la generazione di un label
con l'orario attuale (usato per la gestione file in basso a destra) e
per la gestione dinamica dei colori nelle barre di completamento.

### La server view

La classe `ServerView` è una classe ausiliaria a `MainWindow` che
permette la creazione delle pagine di visualizzazione, aggiunta e
modifica della parte dei servizi. Questa view comunica direttamente col
`Model` per aggiornare i dati e con `MainWindow` per notificare
modifiche ai dati. All'interno di questa classe ci sono anche dei metodi
di verifica e controllo dell'input inserito nei form, che vanno a
segnalare anche eventuali valori duplicati (come l'IP e l'etichetta).

Polimorfismo
------------

L'uso del polimorfismo all'interno del progetto è fortemente presente
nei contenitori per mezzo degli oggetti `DeepPtr<Server>` della
gerarchia. In diversi punti del programma, specialmente nella GUI, si
utilizzano **chiamate polimorfe** a metodi virtuali, che permettono il
recupero di determinate informazioni, come ad esempio il calcolo del
prezzo o la velocità della CPU in relazione ai membri privati usati.

Analogamente, viene fatto largo uso del polimorfismo anche nella parte
di *creazione degli oggetti* al caricamento dei file XML, in cui si
gestisce attraverso una chiamata di deserializzazione l'inserimento nel
contenitore degli oggetti appena creati.

A favore di queste implementazioni, è stato reso necessario il
riconoscimento del tipo di classe usando un metodo virtuale denominato
`staticType()` che fa uso del polimorfismo per ritornare a stringa il
tipo corrente, così da eliminare l'esigenza del *type checking dinamico*
e permettendo il *cast statico*degli oggetti nelle operazioni più
specifiche.

Gestione dati
-------------

Il programma fa uso di **XML** ai fini di salvataggio e caricamento
dati. I dati vengono gestiti ad *alto livello* dal modello che esegue
caricamento e salvataggio delle modifiche effettuate sulla istanza
correntemente aperta e comunica visivamente all'utente i singoli
cambiamenti per mezzo del box messaggi posto in basso a destra
nell'applicazione.

A *basso livello*, invece, è stata implementata una classe denominata
`XMLIO` che esegue la deserializzazione e la serializzazione per server
e nodi. Questa classe fa uso dei **metodi virtuali** appositamente
realizzati nella gerarchia (`dataUnserialize`, `dataSerialize`,
`createObjectFromData()`) al fine di poter leggere o scrivere i file XML
in maniera autonoma e distinta per ogni singola classe concreta, così da
evitarne la necessità di estensione o modifica nel momento in cui si va
ad estendere la gerarchia.

Note generali sulle implementazioni
-----------------------------------

-   Si è scelto di usare un contenitore di tipo **lista** in quanto il
    più adeguato da usufruire per questo ambito, prevedendo di avere un
    numero contenuto di elementi. Da questa motivazione, inoltre, si è
    scelto di risparmiare byte anche per quanto riguarda il tipo di
    alcuni elementi, preferendo `short` a `int` in molti casi.

-   Il template `StaticTypeBuilder<T>` di fondo andrebbe contro il
    principi *SOLID* dal momento che viene **esplicitata la costruzione
    della mappa dei tipi**. In realtà, tale soluzione è stata ritenuta
    la meno onerosa per implementazioni future dal momento che, qualora
    si volesse estendere la gerarchia, sarebbe necessario menzionare
    nella documentazione l'obbligo di creare una sottoclasse interna
    (protetta o privata) da usare per questo scopo. Omettere
    completamente questa funzionalità, d'altra parte, richiederebbe come
    conseguenza l'aggiornamento del XMLIO e del modello, il che va a
    sfavore dell'estendibilità. Altre alternative sarebbero state
    probabilmente più adeguate (es: avere una classe che fa esattamente
    da listener/builder e genera la mappa), ma per questioni di tempo
    richiesto dal progetto e per poca conoscenza sull'argomento si è
    preferito dedicarsi ad altre funzionalità.

-   Per quanto riguarda l'utilizzo del **Model-View**, è opportuno
    ammettere che a programma avviato l'istanza degli adattatori e del
    modello rimangono sempre le stesse, pertanto la cancellazione del
    modello verrà fatta con la distruzione della classe `MainWindow`.
    Ciò è stato rafforzato ulteriormente con l'ausilio del `DeepPtr`,
    impiegato appositamente anche per il modello. Infine, ciascun
    adattatore fa uso di un puntatore normale al modello in base alle
    informazioni che deve ritornare e, allo stesso modo, ad ogni
    distruzione di queste classi si distrugge solamente il puntatore e
    non l'oggetto puntato, come corretto che sia.

-   La **GUI** del programma si è basata in modo particolare
    sull'usabilità, tale per cui fosse semplice, sufficientemente
    intuitiva e al contempo informativa. Per questo motivo, sono state
    aggiunte icone colorate, tooltips, hotkeys e messaggi informativi
    dove è stato possibile, senza peccare sulle funzioni essenziali
    richieste dal progetto. L'applicazione, dunque, parte da una base
    minima, ma solida e aperta a future funzionalità, nei limiti del
    possibile.

-   Nel **Modello** e nella **Gerarchia**, dal momento che si fa
    comunque uso della libreria di Qt per la gestione del XML, è stato
    preferito l'utilizzo di `QString` rispetto a `string`. Questo ha
    reso l'implementazione finale più leggera e con meno conversioni di
    tipo esplicite, senza intaccare sulla separazione modello / vista.
    