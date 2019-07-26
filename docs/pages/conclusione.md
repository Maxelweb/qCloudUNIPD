---
title: Conclusione
layout: post
author: Maxelweb
---


#### [← Torna indietro](../../)


Timeline e riassunto ore di lavoro
----------------------------------

Il progetto è stato inizialmente realizzato con una gerarchia differente
verso dicembre 2018 - gennaio 2019. A marzo il modello è stato
riprogettato, ne sono state eseguite le relative reimplementazioni ad
aprile e si è concluso del tutto a maggio.

A livello di *ore lavorative*, si può dire che ci sono state circa un
**40% di ore lavorative in più** rispetto alle **50 ore** previste dalle
specifiche, dovute alla riprogettazione iniziale e ad un maggiore
affinamento della GUI a livello grafico.

```
  Fase del progetto                          Ore lavorative
  ------------------------------------ --------------------
  Analisi del problema                                    3
  Progettazione modello e GUI                             9
  Apprendimento libreria Qt                               8
  Codifica e implementazione modello                     14
  Codifica e implementazione GUI                         20
  Test generali e Debugging                              10
  Stesura Relazione in LaTeX                              5
  ------------------------------------ --------------------
  Ore totali                                         69 ore
```

Ambiente di lavoro
------------------

Per sviluppare si è fatto uso di **Qt creator** nella versione `5.9.5`,
usando come compilatore `clang 8.1.0`. Lo sviluppo è avvenuto
principalmente su **MacOs 10.13 \"High Sierra\"** (3/4 del tempo) e
**Windows 10** (1/4 del tempo).

A tal proposito è stato integrato un processo di versionamento e
building per mezzo di **Github** e **Travis CI** tale per cui ad ogni
singola modifica è stata fatta una compilazione automatica via cloud su
una macchina **Linux**. Con questa configurazione si è potuto garantire
il funzionamento del programma anche su **Ubuntu 14.04 \"Trusty\"** con
compilatore `g++ 4.8.4`

Compilazione del progetto ed esecuzione
---------------------------------------

Il progetto prevede la compilazione tramite file `.pro` dal momento che
contiene la posizione di tutti i file e identifica la versione specifica
usata per C++, ossia C++11, di cui se ne fa uso nel modello con keywords
apposite.

```
$ qmake qCloudManager.pro
$ make
```

A primo avvio, è possibile creare un nuovo file dal menù oppure può
essere caricato un file dati compatibile. Nella cartella del progetto,
sono stati messi dei **file XML** di esempio che possono essere usati
per il primo caricamento. Alternativamente si può creare un nuovo file
con dentro `<serverList/>` e selezionare il file appena creato.

Note conclusive dello sviluppatore
----------------------------------

In linea di massima, il progetto ha richiesto un impegno moderato, dal
momento che gran parte delle ore sono state spese per la fase di
apprendimento delle classi della libreria di Qt. Una volta capito il
funzionamento però, l'implementazione è stata abbastanza
*straight-forward*.

Una buona porzione di tempo, inoltre, è stata usata nella fase di
progettazione e test della GUI così da curarne l'usabilità, l'aspetto
grafico e le funzionalità. Questo ovviamente non ha tolto ore di lavoro
alla gerarchia, in cui si è cercato di garantire massima estendibiità.
In modo analogo, come richiesto dalle specifiche, è stata curata molto
la parte di *interazione* tra modello e view, al fine di ottenere la
massima indipendenza senza trascurarne le funzionalità.

Concludendo, si può affermare che il progetto è stato abbastanza lungo,
ma il processo di sviluppo è stato preso con molta tranquillità e con
tutti i possibili accorgimenti al fine di ottenere un prodotto
adeguatamente funzionante e stabile.
