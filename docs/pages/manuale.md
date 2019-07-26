---
title: Manuale d'uso
---

#### [← Torna indietro](././)

A prima apertura del programma, viene richiesta la creazione (*File,
Nuovo*) o il caricamento (*File, Apri un nuovo file*) di un file XML che
contiene le configurazioni dei servizi erogati. In alto a destra,
troviamo i **contatori** di avvisi, server e nodi, aggiornati in tempo
reale. Un pochino più sotto al logo è presente una **sezione
informativa** che spiega brevemente le funzionalità del tab corrente ed
eventuali Hot-Keys. In basso a destra, invece, è presente una **barra di
gestione file** che emette notifiche circa la lettura e la scrittura del
file.

Dashboard
---------

La dashboard contiene gli avvisi che riguardano i nodi e le statistiche.
Per quanto riguarda gli **avvisi**, è opportuno tenere sotto controllo i
*Warnings* al fine di aumentare l'efficienza dei singoli nodi. Le
**statistiche**, invece, sono aggiornate in tempo reale e permettono di
visualizzare i guadagni e il costo di mantenimento.

Gestione servizi
----------------

Nella gestione servizi è possibile eseguire azioni di aggiunta, modifica
e cancellazione dei server.

-   Cliccando su un singolo server è possibile aprire i **dettagli** che
    compariranno sulla destra, a fianco della tabella.

-   Con un dettaglio aperto, è possibile eseguirne la **modifica** e
    **cancellazione** con i relativi bottoni che compariranno.

-   Per **chiudere un dettaglio** è sufficiente cliccare sul bottone
    *Chiudi* in alto a destra così da poter riprendere anche la ricerca.

-   Per **aggiungere un server** si deve cliccare sull'apposito bottone
    *Aggiungi* posto in alto a sinistra sulla tabella; ad esso seguirà
    un form laterale con i relativi campi da compilare.

-   Per **ricercare** dei servizi specifici è sufficiente riempire la
    barra di ricerca e/o selezionare un nodo dal menù a tendina posti
    sopra la tabella. Dai risultati ottenuti è possibile eseguire la
    **cancellazione multipla** cliccando sul bottone in basso a sinistra
    rispetto alla tabella.

Gestione nodi
-------------

La gestione dei nodi permette solamente di **visualizzare** in tempo
reale i dettagli di un nodo, tra cui il quantitativo di risorse
impiegate e la percentuale di completamento. *Per vedere più dettagli di
un nodo* è sufficiente cliccare su una entry della tabella e,
successivamente, compariranno sulla destra i dettagli, come per la
gestione servizi.

Opzioni file
------------

Il file che contiene i dati può essere salvato a seguito di modifiche,
salvato con nuovo nome o esportato. Il **salvataggio con nome** comporta
la creazione e la futura modifica di un nuovo file con i server
correntemente modificati. **L'esportazione**, invece, crea semplicemente
un nuovo file a parte, mantenendo le modifiche sul file correntemente
aperto. Per eseguire queste operazioni si può usare la barra superiore
del menù *File* o le **hotkeys** segnalate nel menù. Infine, per avere
maggiori dettagli sul file correntemente aperto, basta cliccare sul menù
*Informazioni*, *File (corrente)*.

Listino prezzi, punti risorse e consumi
---------------------------------------
 
``` 
  Categoria       Oggetto             Prezzo mensile unitario
  --------------- ------------------- -----------------------------

  Generale        vCore (1)                                   2.5 €
  Generale        Ram (0.5GB)                                 2.0 €
  Generale        Disco (10GB)                                1.5 €
  VPS             Base                                        3,5 €
  VPS             Snapshot                                    2,0 €
  VPS             AntiDDoS                                   10,0 €
  Hosting         Base                                        7,5 €
  Hosting         Accesso SSH                                 6.0 €
  Hosting         Turbo boost                                 5.0 €
  Dedicato        Base                                       35,0 €
  Dedicato        vCore (1)                                   3,5 €
  Dedicato        Ram (1GB)                                   4,5 €
  Dedicato        Disco (240GB)                              10,0 €
  Dedicato        Backup attivo                              15,0 €
  Dedicato        Ip aggiuntivo (1)                           5,0 €
```

``` 
  Server       Punti risorse       Nota
  ------------ ------------------- -------------------------

  Hosting               1          \-
  VPS                  2-4         Varia in base ai vCores
  Dedicato              4          \-
```

``` 
  Server       Velocità       Nota
  ------------ -------------- ----------------------  
  Hosting         1.2 Ghz     +25% con Turbo Boost
  VPS             2.1 Ghz     -5% con AntiDDoS
  Dedicato        3.4 Ghz     \-
```

```
  Base approssimativa       Stato         Costo
  ------------------------- ------------- ------------

  250 W / 4 punti risorsa    Idle (25%)    0.089 KW/h
  250 W / 4 punti risorsa    Full (100%)   0.089 KW/h
```