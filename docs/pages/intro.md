---
title: Introduzione al progetto
layout: post
author: Maxelweb
---

#### [← Torna indietro](../../)

Abstract
--------

Si vuole realizzare un programma per la gestione dell'infrastruttura
hardware di una azienda denominata **qCloud Solutions** che offre
servizi Web. L'azienda possiede una **server farm** in cui vengono
erogati dei servizi che si differenziano per varie tipologie in base
alle necessità dei clienti. Ciascun server ha un costo base fissato e un
costo di risorse che varia in base alla configurazione.\
I server messi a disposizioni si differenziano per tre tipologie
principali:

- **Hosting:** soluzione mirata per uno o più siti web e completamente gestita lato
    server da parte dell'azienda. Le risorse richieste sono molto basse.

- **VPS (virtual private server):** soluzione intermedia che permette di avere un server con risorse
    scalabili ad un costo contenuto. Usato su larga scala e solo su
    ambienti virtualizzati.

- **Server Dedicati:** soluzione molto onerosa che viene impiegata soprattutto per grossi
    progetti che richiedono molte risorse.

Ogni servizio erogato fa parte di un nodo hardware. Un *nodo hardware*
costituisce una serie di macchine fisiche che si differenziano per la
presenza di *virtualizzazione* del sistema operativo e per il
quantitativo totale di risorse massime.

Per agevolare la gestione ed efficienza dei servizi, l'azienda ha
adottato un sistema a punteggi per ogni singolo servizio erogato in
relazione al nodo di appartenenza: un server può *pesare* da 1 a 4 punti
risorse. Quando un nodo hardware supera il numero di punti risorse
disponibili, viene segnalato tramite *warning*, così come quando non
viene minimamente usufruito. Sfruttare un nodo più del suo potenziale è
permesso, ma ciò può causare troppo carico sulle macchine fisiche e
provocare downtime.

Infine, ciascun nodo hardware ha un *consumo variabile di KW/H* basato
sulla quantità di macchine attive presenti. Il tutto si differenzia sia
a carico minimo (*idle*) che a pieno carico (*full*). Tali statistiche
permettono di fare previsioni sul costo di mantenimento dei servizi in
relazione al guadagno.

Funzionalità del programma
--------------------------

Il programma permette di gestire i nuovi server erogati e di assegnarli
a nodi hardware predefiniti. In particolare, sono presenti le seguenti
funzionalità:

-   Caricamento, salvataggio ed esportazione dei dati correnti

-   Aggiunta di un nuovo servizio (server)

-   Modifica di un servizio correntemente attivo

-   Eliminazione di un singolo servizio

-   Eliminazione multipla basata su una ricerca specifica

-   Ricerca specifica per nodo, tipo, etichetta e indirizzo IP del
    servizio

-   Visualizzazione veloce delle caratteristiche di un server tramite
    tabella

-   Visualizzazione dei nodi hardware e della relativa percentuale di
    completamento

-   Visualizzazione degli avvisi (*warning*) dei nodi

-   Visualizzazione delle statistiche globali in tempo reale in base
    alla configurazione corrente

Premesse: dall'idea al progetto
-------------------------------

L'idea di fondo del programma si basa sul fatto che l'applicazione
permetta di gestire tutto il servizio aziendale principale attraverso
una infrastruttura di rete già predisposta, che ha le seguenti premesse:

-   l'infrastruttura ricava in automatico i nodi e i server presenti con
    le relative specifiche e genera dei file XML appositi;

-   l'aggiornamento di un server in termini di scalabilità è fatto con
    subroutine interne automatizzate;

-   l'accesso è protetto e l'usufrutto è dedicato solo ai dipendenti
    aziendali.

Da queste premesse si è costruito e adattato il progetto affinché si
potesse ottenere il risultato più vicino a quello pensato. A livello
implementativo si è scelto di lasciare in *sola lettura* il file XML che
contiene i *nodi*, mentre i server possono essere caricati con un file
esterno, di cui fa uso la parte di gestione file.
