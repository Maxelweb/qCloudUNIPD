#include "mainwindow.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QLineEdit>
#include <QPixmap>
#include <QListWidget>
#include <QTabWidget>
#include <QGroupBox>
#include <QLabel>
#include <QHeaderView>
#include <QTableView>
#include <QScrollArea>
#include <QProgressBar>
#include <QComboBox>
#include <QCloseEvent>
#include <QAbstractTableModel>
#include <QModelIndex>

#include "extra/qlabeltime.h"
#include "extra/qchungusbar.h"

#include "../model/qtablenodesadapter.h"
#include "../model/qtableserveradapter.h"
#include "../model/qfiltersearchserver.h"
#include "serverview.h"


void MainWindow::populate_nodesComboBox(QComboBox * selection)
{
    selection->clear();
    selection->addItem("Tutti i nodi");
    auto nodi = model->getNodesIdList();
    for(auto it = nodi.begin(); it != nodi.end(); ++it)
        selection->addItem(QString(*it));
}

void MainWindow::create_notificationBar()
{
    cleanLayout(layoutTopRightNotification);

    warnImage = new QLabel();
    warnImage->setPixmap(QPixmap(model->countWarnings() <= 0 ?
                                 ":res/styles/icons/no-notification.svg"
                                 : ":res/styles/icons/notification.svg").scaled(30, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    notWarnings = new QLabel(QString::number(model->countWarnings()));
    warnImage->setProperty("not-icon", true);
    notWarnings->setProperty("class", "not-text");
    notWarnings->setToolTip("Numero di Avvisi");

    QLabel * image2 = new QLabel();
    image2->setPixmap(QPixmap(":res/styles/icons/server.svg").scaled(30, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    notServers = new QLabel(QString::number(model->countServer()));
    image2->setProperty("not-icon", true);
    notServers->setProperty("class", "not-text not-light");
    notServers->setToolTip("Numero di Server");

    QLabel * image3 = new QLabel();
    image3->setPixmap(QPixmap(":res/styles/icons/nodes.svg").scaled(30, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    notNodes = new QLabel(QString::number(model->countNodes()));
    image3->setProperty("not-icon", true);
    notNodes->setProperty("class", "not-text not-light");
    notNodes->setToolTip("Numero di Nodi");

    notWarnings->setMaximumHeight(30);
    notServers->setMaximumHeight(30);
    notNodes->setMaximumHeight(30);

    layoutTopRightNotification->addWidget(warnImage, 0, 0);
    layoutTopRightNotification->addWidget(notWarnings, 0, 1);
    layoutTopRightNotification->addWidget(image2, 0, 2);
    layoutTopRightNotification->addWidget(notServers, 0, 3);
    layoutTopRightNotification->addWidget(image3, 0, 4);
    layoutTopRightNotification->addWidget(notNodes, 0, 5);
}

unsigned int MainWindow::getServerSelectionRow() const
{
    return static_cast<unsigned int>(getServerSelection().row());
}

QModelIndex MainWindow::getServerSelection() const
{
    return serverProxy->mapToSource(serverTable->currentIndex());
}

void MainWindow::setFileNotification(const QString & str)
{
    textFooterRight->setText(str);
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      model(new Model()),
      nodesTable(new QTableView()),
      serverTable(new QTableView()),
      layoutGeneralPage(new QHBoxLayout()),
      tabs(new QTabWidget()),
      sectionInfoTitle(new QLabel()),
      viewInfo(new QVBoxLayout()),
      layoutBody(new QHBoxLayout()),
      layoutTopRightNotification(new QGridLayout()),
      textFooterRight(new QLabelTime()),
      fSearch(new QLineEdit()),
      fSelect(new QComboBox())
{

    // Setup della applicazione
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
    setWindowIcon(QIcon(":res/styles/images/icon.ico"));
    setWindowTitle(model->getProgramName());
    setMinimumSize(1024, 720);


    // LOGO
    QPixmap logo(":/res/styles/images/logo.png");

    QLabel * logoImage = new QLabel();
    logoImage->setPixmap(logo.scaled(200, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoImage->setMaximumWidth(220);
    logoImage->setProperty("logo", true);


    // LABELS
    QLabel * tTitle = new QLabel(QString("<h1>%1</h1> Gestione server, nodi e fatturazione").arg(model->getProgramName()));
    tTitle->setProperty("main-title", true);
    QLabel * tFooter = new QLabel("<small>Progetto di Programmazione ad Oggetti - UniPD</small>");
    tFooter->setProperty("class", "footer-left");
    textFooterRight->setText("Pronto (aprire un file per iniziare)");
    textFooterRight->setProperty("class", "footer-right");
    QPixmap iconFooterRight = QPixmap(":res/styles/icons/file.svg");
    QLabel * labelFooterRight = new QLabel();
    labelFooterRight->setPixmap(iconFooterRight.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    labelFooterRight->setMaximumWidth(20);
    labelFooterRight->setProperty("class", "footer-right-icon");


    // MENÙ e MENUBAR
    QMenuBar* menuBar = new QMenuBar();
    QMenu* menu = new QMenu("File", menuBar);
    QAction* newAction = new QAction("Nuovo", menu);
        newAction->setShortcut(Qt::CTRL | Qt::Key_N);
    QAction* openAction = new QAction("Apri un nuovo file", menu);
        openAction->setShortcut(Qt::CTRL | Qt::Key_O);
    QAction* saveAction = new QAction("Salva", menu);
        saveAction->setShortcut(Qt::CTRL | Qt::Key_S);
    QAction* saveAsAction = new QAction("Salva con nome", menu);
        saveAsAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S);
    QAction* exportAction = new QAction("Esporta dati correnti", menu);
        exportAction->setShortcut(Qt::CTRL | Qt::Key_E);

    QMenu* menu2 = new QMenu("Informazioni", menuBar);
    QAction* fileInfoAction = new QAction("File (corrente)", menu2);
    QAction* infoAction = new QAction("Crediti e Licenze", menu2);

    QMenu* menu3 = new QMenu("Altro", menuBar);
    QAction* exitAction = new QAction("Esci", menu3);

    // Setup del menù
    menuBar->addMenu(menu);
        menu->addAction(newAction);
        menu->addAction(openAction);
        menu->addAction(saveAction);
        menu->addAction(saveAsAction);
        menu->addAction(exportAction);
    menuBar->addMenu(menu2);
        menu2->addAction(fileInfoAction);
        menu2->addAction(infoAction);
    menuBar->addMenu(menu3);
        menu3->addAction(exitAction);


    // LAYOUT
    QVBoxLayout *layoutMain = new QVBoxLayout();
    QVBoxLayout *layoutContainer = new QVBoxLayout();

    // - Menù superiore
    QVBoxLayout *layoutTopMenu = new QVBoxLayout();
    layoutMain->addLayout(layoutTopMenu);
    layoutMain->addLayout(layoutContainer);

    // - Titolo superiore con logo
    QHBoxLayout *layoutTop = new QHBoxLayout();
    layoutTop->setProperty("header", true);

    QVBoxLayout *layoutTopRightTitle = new QVBoxLayout();
    QVBoxLayout *layoutTopLeftLogo = new QVBoxLayout();
    QHBoxLayout *layoutHead = new QHBoxLayout();

    layoutContainer->addLayout(layoutTop);
    layoutTop->addLayout(layoutTopLeftLogo);
    layoutTop->addLayout(layoutTopRightTitle);
    layoutTop->addLayout(layoutTopRightNotification);
    layoutTop->setAlignment(layoutTopRightNotification, Qt::AlignRight);

    // - Head + Body
    layoutContainer->addLayout(layoutHead);
    layoutContainer->addLayout(layoutBody);

    // - Footer
    QHBoxLayout *layoutFooter = new QHBoxLayout();
    QHBoxLayout *layoutFooterRight = new QHBoxLayout();
    layoutContainer->addLayout(layoutFooter);


    // TAB PRINCIPALI

    // Dashboard
    QWidget * generalPage = new QWidget();
    layoutGeneralPage->setSizeConstraint(QLayout::SetMaximumSize);
    generalPage->setLayout(layoutGeneralPage);


    // Server Page
    QWidget * serverPage = new QWidget();
    QVBoxLayout * layoutServerPage = new QVBoxLayout();
    layoutServerPage->setSizeConstraint(QLayout::SetMaximumSize);
    serverPage->setLayout(layoutServerPage);
    QLabel * stitle = new QLabel("<h3>Server <small>attivi</small></h3>");
    stitle->setProperty("subtitle", true);
    stitle->setMaximumHeight(60);

    // - Bottoni e Ricerca
    QPushButton * bAdd = new QPushButton(QIcon(":/res/styles/icons/plus.svg"), "Aggiungi");
    QPushButton * bDeleteAll = new QPushButton(QIcon(":/res/styles/icons/deleteAll.svg"), "Elimina server correnti");
    fSearch = new QLineEdit();
    fSelect = new QComboBox();
    QWidget* emptyFix = new QWidget();
    emptyFix->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
    fSearch->setPlaceholderText("Ricerca per tipo, etichetta o ip");
    fSearch->setMinimumSize(200, 20);
    fSearch->setProperty("class","searchBox");
    QHBoxLayout * toolServerPage = new QHBoxLayout();
    toolServerPage->addWidget(bAdd, 2, Qt::AlignLeft);
    toolServerPage->addWidget(emptyFix, 2, Qt::AlignRight);
    toolServerPage->addWidget(fSearch);
    toolServerPage->addWidget(fSelect);

    layoutServerPage->addWidget(stitle);
    layoutServerPage->addLayout(toolServerPage);
    layoutServerPage->addWidget(serverTable);
    layoutServerPage->addWidget(bDeleteAll, 1, Qt::AlignLeft);

    // Nodes page
    QWidget * nodesPage = new QWidget();
    QVBoxLayout * layoutNodesPage = new QVBoxLayout();
    layoutNodesPage->setSizeConstraint(QLayout::SetMaximumSize);
    nodesPage->setLayout(layoutNodesPage);
    QLabel * stitle2 = new QLabel("<h3>Nodi hardware <small>attivi</small></h3>");
    stitle2->setProperty("subtitle", true);
    stitle2->setMaximumHeight(60);
    layoutNodesPage->addWidget(stitle2);
    layoutNodesPage->addWidget(nodesTable);


    // Tab widget (laterale sinistro)
    tabs->insertTab(0, generalPage, QIcon(":/res/styles/icons/home.svg"), "Dashboard");
    tabs->insertTab(1, serverPage, QIcon(":/res/styles/icons/services.svg"), "Gestione Servizi");
    tabs->insertTab(2, nodesPage, QIcon(":/res/styles/icons/nodes.svg"), "Gestione Nodi");

    tabs->setTabEnabled(1, false);
    tabs->setTabEnabled(2, false);


    // POSIZIONAMENTO ELEMENTI
    layoutTopMenu->addWidget(menuBar);
    layoutTopLeftLogo->addWidget(logoImage);
    layoutTopRightTitle->addWidget(tTitle);


    // Top per le info
    QVBoxLayout * tl = new QVBoxLayout();
    QVBoxLayout * tr = new QVBoxLayout();
    QPixmap infoIcon(":/res/styles/icons/info.svg");
    QLabel * infoImage = new QLabel();
    infoImage->setPixmap(infoIcon.scaled(21, 21, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    infoImage->setMaximumWidth(28);
    infoImage->setProperty("section-info-icon", true);
    infoImage->setAlignment(Qt::AlignTop);
    layoutHead->addLayout(tl);
    layoutHead->addLayout(tr);
    tl->addWidget(infoImage);
    tr->addWidget(sectionInfoTitle);
    sectionInfoTitle->setTextFormat(Qt::RichText);
    sectionInfoTitle->setWordWrap(true);
    sectionInfoTitle->setProperty("section-desc", true);

    layoutBody->addWidget(tabs);
    layoutBody->addLayout(viewInfo);

    layoutFooter->addWidget(tFooter, 0, Qt::AlignLeft);

    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);

    layoutFooter->addLayout(layoutFooterRight);
    layoutFooterRight->addWidget(empty);
    layoutFooterRight->addWidget(textFooterRight, 0, Qt::AlignRight);
    layoutFooterRight->addWidget(labelFooterRight, 0, Qt::AlignRight);


    // CONFIGURAZIONE TABELLE
    nodesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    serverTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    nodesTable->verticalHeader()->hide();
    nodesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    nodesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    nodesTable->setAlternatingRowColors(true);
    nodesTable->setProperty("table-nodes", true);
    nodesTable->horizontalHeader()->setSectionsClickable(false);

    serverTable->verticalHeader()->hide();
    serverTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    serverTable->setSelectionMode(QAbstractItemView::SingleSelection);
    serverTable->setAlternatingRowColors(true);
    serverTable->setProperty("table-server", true);
    serverTable->horizontalHeader()->setSectionsClickable(false);


    // CARICAMENTO MODELLO
    nodesAdapter = new QTableNodesAdapter(model.target(), this);
    nodesTable->setModel(nodesAdapter);

    serverAdapter = new QTableServerAdapter(model.target(), this);
    serverProxy = new QFilterSearchServer(this, fSelect);
    serverProxy->setSourceModel(serverAdapter);
    serverTable->setModel(serverProxy);


    // CONNESSIONI E SEGNALI

    connect(newAction, SIGNAL(triggered()), this, SLOT(pokeToCreateNewData()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openDialogToLoadData()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(pokeModelToSaveData()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(pokeModelToSaveAsData()));
    connect(exportAction, SIGNAL(triggered()), this, SLOT(pokeModelToExportData()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(infoAction, SIGNAL(triggered()), this, SLOT(showInfoBox()));
    connect(fileInfoAction, SIGNAL(triggered()), this, SLOT(showFileInfoBox()));
    connect(nodesTable,SIGNAL(clicked(QModelIndex)), this, SLOT(showNodeDetails()));  
    connect(serverTable,SIGNAL(clicked(QModelIndex)), this, SLOT(showServerDetails()));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(detailAutoCleaner()));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(updateSectionTitle()));
    connect(bAdd, SIGNAL(clicked(bool)), this, SLOT(showServerAddDetails()));
    connect(fSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateSearchFilter()));
    connect(fSearch, SIGNAL(textChanged(QString)), this, SLOT(UpdateSearchFilter()));
    connect(bDeleteAll, SIGNAL(clicked(bool)), this, SLOT(DeleteServerResults()));

    tabs->setCurrentIndex(0);
    updateGeneralPage();
    updateSectionTitle();
    UpdateSearchFilter();

    create_notificationBar();

    setLayout(layoutMain);
}

void MainWindow::closeEvent(QCloseEvent *evento)
{
    // Se il file non è stato modificato o non è presente

    if(model->saveFileUpdated() || model->getFilepath().isEmpty())
        return evento->accept();

    // Altrimenti richiedo conferma per salvataggio

    QMessageBox::StandardButton response
            = QMessageBox::question(this, "Conferma chiusura programma",
                                    "Vuoi salvare i dati modificati prima di chiudere?",
                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                    QMessageBox::Yes);

    if(response == QMessageBox::Cancel)
        evento->ignore();
    else
    {
        if(response == QMessageBox::Yes)
            pokeModelToSaveData();

        evento->accept();
    }
}

void MainWindow::pokeToCreateNewData()
{
    checkUnsavedData();

    QString path = openDialogToNewSaveFile();

    if(path.isEmpty())
        return;

    if(model->newFile(path))
        pokeModelToLoadData(path);
    else
        setFileNotification("<u>Errore:</u> impossibile inizializzare il nuovo file");
}

void MainWindow::pokeModelToLoadData(const QString & f)
{
    model->loadFile(f);

    if(model->saveFileUpdated())
    {
        model->stats_update();
        serverAdapter->dataRefresh();
        nodesAdapter->dataRefresh();
        updateGeneralPage();
        updateNotification();
        detailAutoCleaner();
        UpdateSearchFilter();

        populate_nodesComboBox(fSelect);

        // Primo caricamento
        if(!tabs->isTabEnabled(1))
        {
            tabs->setTabEnabled(1, true);
            tabs->setTabEnabled(2, true);
        }

        setWindowTitle(model->getProgramName() + " - " + QFileInfo(f).fileName());
        setFileNotification("Nuovi dati <u>caricati</u>");
    }
    else
        setFileNotification("<b>Errore:</b> nuovi dati <u>non</u> caricati");
}

void MainWindow::pokeModelToSaveData()
{
    if(!checkFilepath())
        return;

    model->saveFile();

    if(model->saveFileUpdated())
        setFileNotification("Dati <u>salvati</u>");
    else
        setFileNotification("<b>Errore:</b> dati <u>non</u> salvati");
}

void MainWindow::pokeModelToSaveAsData()
{
    if(!checkFilepath())
        return;

    QString newfile = openDialogToNewSaveFile();

    if(newfile.isEmpty())
        return;

    model->saveFile(newfile);

    if(model->saveFileUpdated())
    {
        model->setFilepath(newfile);
        setWindowTitle(model->getProgramName() + " - " + QFileInfo(newfile).fileName());
        setFileNotification("Dati <u>salvati</u> in un nuovo file");
    }
    else
        setFileNotification("<b>Errore:</b> dati <u>non</u> salvati in un nuovo file");
}

void MainWindow::pokeModelToExportData()
{
    if(!checkFilepath())
        return;

    checkUnsavedData();

    QString newfile = openDialogToNewSaveFile();

    if(newfile.isEmpty())
        return;

    if(model->exportFile(newfile))
        setFileNotification("Dati <u>esportati</u> in un nuovo file");
    else
        setFileNotification("<b>Errore:</b> dati <u>non</u> esportati in un nuovo file");
}

void MainWindow::openDialogToLoadData()
{
    checkUnsavedData();

    QString filename = QFileDialog::getOpenFileName(this,
                                            "Carica file",
                                            model->getFilepath().isEmpty()
                                                    ? QDir::homePath()
                                                    : QFileInfo(model->getFilepath()).path(),
                                            "XML (*.xml)");
    if(!filename.isEmpty())
        pokeModelToLoadData(filename);

}

QString MainWindow::openDialogToNewSaveFile()
{
    return QFileDialog::getSaveFileName(this,
                                        "Salva su un nuovo file",
                                        model->getFilepath().isEmpty()
                                            ? QDir::homePath()
                                            : QFileInfo(model->getFilepath()).path(),
                                        "XML (*.xml)");
}

void MainWindow::checkUnsavedData()
{
    if(!model->saveFileUpdated() && !model->getFilepath().isEmpty())
    {
        QMessageBox::StandardButton response
            = QMessageBox::question(this, "Salvataggio dati",
                                    "Vuoi salvare i dati modificati prima di continuare?",
                                    QMessageBox::No | QMessageBox::Yes,
                                    QMessageBox::Yes);

        if(response == QMessageBox::Yes)
            pokeModelToSaveData();
    }
}

bool MainWindow::checkFilepath()
{
    if(model->getFilepath().isEmpty())
    {
        QMessageBox box(QMessageBox::Information,
                        "Informazione",
                        "Nessun file aperto da salvare o esportare.",
                        QMessageBox::Ok);
        box.exec();
        return false;
    }

    return true;
}

void MainWindow::updateNotification()
{
    notWarnings->setText(QString::number(model->countWarnings()));
    notServers->setText(QString::number(model->countServer()));
    notNodes->setText(QString::number(model->countNodes()));
    warnImage->setPixmap(
                QPixmap(model->countWarnings() <= 0 ?
                    ":res/styles/icons/no-notification.svg"
                    : ":res/styles/icons/notification.svg")
                .scaled(30, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::showServerDetails()
{
    if(getServerSelection().row() < 0)
        return;

    // Pulisco il layout e blocco la ricerca fintanto che la view è aperta
    cleanLayout(viewInfo);
    fSearch->setEnabled(false);
    fSelect->setEnabled(false);

    // Genero il layout a griglia per i dettagli
    Server * s = model->getServer(getServerSelectionRow());
    QLayout * grid = ServerView(model.target(), s, this).readServerDetail();

    // Titolo del dettaglio
    QLabel * title = new QLabel(QString("<h3><img src=':res/styles/icons/fixed/server.png'> "
                                        "Dettaglio <u>%1</u></h3>").arg(s->getLabel()));
    title->setMinimumWidth(380);
    title->setMaximumHeight(50);
    title->setProperty("subtitle", true);


    // Bottoni modifica e cancellazione
    QPushButton * bEdit = new QPushButton(QIcon(":/res/styles/icons/edit.svg"), "Modifica");
    QPushButton * bDelete = new QPushButton(QIcon(":/res/styles/icons/delete.svg"), "Elimina");
    connect(bEdit,SIGNAL(clicked(bool)), this, SLOT(showServerEditDetails()));
    connect(bDelete,SIGNAL(clicked(bool)), this, SLOT(DeleteServer()));

    // Bottone di chiusura
    QPushButton * closeDetail = new QPushButton(QIcon(":res/styles/icons/back-right.svg"), "Chiudi");
    closeDetail->setShortcut(Qt::CTRL | Qt::Key_K);
    connect(closeDetail, SIGNAL(clicked()), this, SLOT(detailAutoCleaner()));

    QHBoxLayout * tools = new QHBoxLayout();
    tools->addWidget(bEdit);
    tools->addWidget(bDelete);
    tools->addWidget(closeDetail, 2, Qt::AlignRight);

    viewInfo->addLayout(tools);
    viewInfo->addWidget(title);
    viewInfo->addLayout(grid);
}

void MainWindow::showServerEditDetails()
{
    if(getServerSelection().row() < 0)
        return;

    // Pulisco il layout e blocco la ricerca fintanto che la view è aperta
    cleanLayout(viewInfo);
    fSearch->setEnabled(false);
    fSelect->setEnabled(false);

    Server * s = model->getServer(getServerSelectionRow());
    ServerView * sw = new ServerView(model.target(), s, this);
    QLayout * grid = sw->editServerDetail();

    connect(sw, SIGNAL(pokeUpdateServer()), this, SLOT(UpdateServer()));

    // Titolo del dettaglio
    QLabel * title = new QLabel(QString("<h3><img src=':res/styles/icons/fixed/server.png'> "
                                        "Modifica <u>%1</u></h3>").arg(s->getLabel()));
    title->setMinimumWidth(380);
    title->setMaximumHeight(50);
    title->setProperty("subtitle", true);


    // Bottone di chiusura
    QPushButton * closeDetail = new QPushButton(QIcon(":res/styles/icons/back-right.svg"), "Annulla e chiudi");
    closeDetail->setShortcut(Qt::CTRL | Qt::Key_K);
    connect(closeDetail, SIGNAL(clicked()), this, SLOT(detailAutoCleaner()));

    QHBoxLayout * tools = new QHBoxLayout();
    tools->addWidget(closeDetail, 2, Qt::AlignRight);

    viewInfo->addLayout(tools);
    viewInfo->addWidget(title);
    viewInfo->addLayout(grid);
}

bool MainWindow::showServerDeleteConfirm()
{
    QMessageBox::StandardButton box
            = QMessageBox::question(this, "Conferma cancellazione",
                                    "Confermi la cancellazione di questo server?", QMessageBox::Yes|QMessageBox::No);
    return (box == QMessageBox::Yes);
}

bool MainWindow::showServerDeleteAllConfirm()
{
    QMessageBox::StandardButton box
            = QMessageBox::question(this, "Conferma cancellazione multipla",
                                    "Confermi la cancellazione di <u>tutti i server</u> visualizzati "
                                    "correntemente nella tabella?", QMessageBox::Yes|QMessageBox::No);
    return (box == QMessageBox::Yes);
}

void MainWindow::showServerAddDetails()
{
    // Pulisco il layout e lascio abilitata la ricerca
    cleanLayout(viewInfo);
    fSearch->setEnabled(true);
    fSelect->setEnabled(true);
    serverTable->clearSelection();

    // Genero il layout a griglia per i dettagli
    ServerView * sw = new ServerView(model.target(), nullptr, this);
    QLayout * grid = sw->addServerDetail();
    connect(sw, SIGNAL(pokeAddServer()), this, SLOT(AddServer()));

    // Titolo nuovo server
    QLabel * title = new QLabel(QString("<h3><img src=':res/styles/icons/fixed/server.png'> "
                                        "Aggiungi nuovo server</h3>"));
    title->setMinimumWidth(380);
    title->setMaximumHeight(50);
    title->setProperty("subtitle", true);

    // Bottone di annullamento e chiusura
    QPushButton * closeDetail = new QPushButton(QIcon(":res/styles/icons/back-right.svg"), "Annulla e chiudi");
    closeDetail->setShortcut(Qt::CTRL | Qt::Key_K);
    connect(closeDetail, SIGNAL(clicked()), this, SLOT(detailAutoCleaner()));
    QHBoxLayout * tools = new QHBoxLayout();
    tools->addWidget(closeDetail, 2, Qt::AlignRight);

    viewInfo->addLayout(tools);
    viewInfo->addWidget(title);
    viewInfo->addLayout(grid);
}

void MainWindow::showNodeDetails()
{
    cleanLayout(viewInfo);

    QGridLayout * grid = new QGridLayout();

    grid->addWidget(new QLabel("<b>Completamento:</b>"), 0, 0);
    grid->addWidget(new QLabel("<b>Etichetta:</b>"), 1, 0);
    grid->addWidget(new QLabel("<b>Ambiente:</b>"), 2, 0);
    grid->addWidget(new QLabel("<b>Servizi attivi:</b>"), 3, 0);
    grid->addWidget(new QLabel("<b>Limite Risorse:</b>"), 4, 0);
    grid->addWidget(new QLabel("<b>Risorse usate:</b>"), 5, 0);
    grid->addWidget(new QLabel("<b>Risorse rimanenti:</b>"), 6, 0);
    grid->addWidget(new QLabel("<b>Consumo Idle (KW):</b>"), 7, 0);
    grid->addWidget(new QLabel("<b>Consumo Normale (KW):</b>"), 8, 0);
    grid->addWidget(new QLabel("<b>Costo mensile (EUR):</b>"), 9, 0);
    grid->addWidget(
           new QLabel("<b>Nota:</b> costo approssimato di 250W ogni 4 risorse e 0.089 EUR / KWH"),
                10, 0, 2, 2);

    unsigned int i = static_cast<unsigned int>(nodesTable->selectionModel()->currentIndex().row());
    Node * n = model->getNode(i);
    char nid = n->getId();

    QChungusBar * pro = new QChungusBar(0, n->getLimits(),
                                           model->node_countUsedResources(nid));
    pro->applyCustomStyle(true);

    grid->addWidget(pro, 0, 1);
    grid->addWidget(new QLabel(n->getName()), 1, 1);
    grid->addWidget(new QLabel(n->isVirtualized() ? "Virtualizzato" : "Dedicato"), 2, 1);
    grid->addWidget(new QLabel(QString::number(model->node_countServer(nid))), 3, 1);
    grid->addWidget(new QLabel(QString::number(n->getLimits())), 4, 1);
    grid->addWidget(new QLabel(QString::number(model->node_countUsedResources(nid))), 5, 1);
    grid->addWidget(new QLabel(QString::number(model->node_countRemainingResources(nid))), 6, 1);
    grid->addWidget(new QLabel(QString::number(n->getConsumo(true))), 7, 1);
    grid->addWidget(new QLabel(QString::number(n->getConsumo(false))), 8, 1);
    grid->addWidget(new QLabel(QString::number(n->getCosto(DefaultKWHCost, true))), 9, 1);

    QLabel * title = new QLabel(QString("<h3><img src=':res/styles/icons/fixed/node.png'> "
                                        "Dettaglio <u>Nodo %1</u></h3>").arg(n->getId()));
    title->setMaximumHeight(50);
    title->setProperty("subtitle", true);

    QPushButton * closeDetail = new QPushButton(QIcon(":res/styles/icons/back-right.svg"), "Chiudi");
    closeDetail->setShortcut(Qt::CTRL | Qt::Key_K);
    connect(closeDetail, SIGNAL(clicked()), this, SLOT(detailAutoCleaner()));

    viewInfo->addWidget(closeDetail, 0, Qt::AlignRight);
    viewInfo->addWidget(title);
    viewInfo->addLayout(grid);
}

void MainWindow::updateGeneralPage()
{
    cleanLayout(layoutGeneralPage);

    QGridLayout * layoutOwo = new QGridLayout();

    QVBoxLayout * layAvvisi = new QVBoxLayout();
    QHBoxLayout * layStats = new QHBoxLayout();

    QGroupBox* avvisi = new QGroupBox("Avvisi");
    QGroupBox* stats = new QGroupBox("Statistiche");

    avvisi->setLayout(layAvvisi);
    avvisi->setMaximumWidth(420);
    stats->setLayout(layStats);

    QLabel * title = new QLabel("<h3>Dashboard <small>avvisi & statistiche</small></h3>");
    title->setProperty("subtitle", true);
    title->setMaximumHeight(50);
    layoutOwo->addWidget(title, 0, 0, 1, 2);
    layoutOwo->addWidget(avvisi, 1, 0);
    layoutOwo->addWidget(stats, 1, 1);


    QLabel * descAvvisi = new QLabel(QString("Gli avvisi riguardano la gestione e l'efficienza dei nodi. "
                                             "Attualmente ci sono <b>%1 warnings</b> da verificare.").arg(model->countWarnings()));
    descAvvisi->setWordWrap(true);
    layAvvisi->setSpacing(4);

    layAvvisi->addWidget(descAvvisi);


    // Scroll area dashboard

    QWidget * innerAvvisi = new QWidget();
    QScrollArea *scrollAvvisi = new QScrollArea();
    QVBoxLayout *layInnerAvvisi = new QVBoxLayout(innerAvvisi);

    scrollAvvisi->setWidget(innerAvvisi);
    scrollAvvisi->setWidgetResizable(true);
    scrollAvvisi->setProperty("resize", true);
    scrollAvvisi->setMaximumWidth(400);
    scrollAvvisi->setAlignment(Qt::AlignTop);
    scrollAvvisi->setBackgroundRole(QPalette::Light);

    layAvvisi->addWidget(scrollAvvisi);

    std::vector<QString> warnings = model->getWarnings();
    for(auto it = warnings.begin(); it != warnings.end(); ++it)
    {
        QLabel * avviso = new QLabel(QString("<img src=':res/styles/icons/fixed/warning.png'> %1").arg(*it));
        avviso->setWordWrap(true);
        avviso->setProperty("warn-text", true);
        layInnerAvvisi->addWidget(avviso);
    }

    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layInnerAvvisi->addWidget(empty);

    // Statistiche

    QGridLayout * layoutDetailedStats = new QGridLayout();
    layStats->addLayout(layoutDetailedStats);

    unsigned short percentRes = static_cast<unsigned short>(model->stats_getPercentageResourcesUsed());

    QChungusBar * barraBella = new QChungusBar(0, 100, percentRes);
    barraBella->applyCustomStyle();
    barraBella->setMinimumWidth(100);
    barraBella->setMaximumWidth(350);


    layoutDetailedStats->addWidget(new QLabel("<b>Risorse usate:</b> "), 0, 0);
    QString totalRes = QString("<b>%1 / %2</b> (%3%)")
                                .arg(model->stats_getTotalUsedResources())
                                .arg(model->stats_getTotalResources())
                                .arg(percentRes);
    layoutDetailedStats->addWidget(new QLabel(totalRes), 0, 1, 1, 1, Qt::AlignLeft);
    layoutDetailedStats->addWidget(barraBella, 1, 1);
    layoutDetailedStats->addWidget(new QLabel("<b>Servizio più comprato:</b>"), 2, 0);
    layoutDetailedStats->addWidget(new QLabel("<b>Nodo più usato:</b>"), 3, 0);
    layoutDetailedStats->addWidget(new QLabel("<b>Nodo meno usato:</b>"), 4, 0);
    layoutDetailedStats->addWidget(new QLabel("<b>Costo e consumo totale nodi (idle):</b>"), 5, 0);
    layoutDetailedStats->addWidget(new QLabel("<b>Costo e consumo totale nodi (full):</b>"), 6, 0);
    layoutDetailedStats->addWidget(new QLabel("<b>Guadagno totale (mensile):</b>"), 7, 0);
    layoutDetailedStats->addWidget(new QLabel("<b>Guadagno netto medio (mensile):</b>"), 8, 0);

    layoutDetailedStats->addWidget(new QLabel(model->stats_getMostUsedService()), 2, 1);
    layoutDetailedStats->addWidget(new QLabel(QString(model->stats_getMostUsedNode())), 3, 1);
    layoutDetailedStats->addWidget(new QLabel(QString(model->stats_getLessUsedNode())), 4, 1);
    layoutDetailedStats->addWidget(new QLabel(QString("%1 € - %2 W")
                                              .arg(model->stats_getTotalNodesCost(DefaultKWHCost, true))
                                              .arg(model->stats_getTotalNodesConsume(true)))
                                   , 5, 1);
    layoutDetailedStats->addWidget(new QLabel(QString("%1 € - %2 W")
                                              .arg(model->stats_getTotalNodesCost(DefaultKWHCost, false))
                                              .arg(model->stats_getTotalNodesConsume(false)))
                                   , 6, 1);
    layoutDetailedStats->addWidget(new QLabel(QString("%1 €")
                                              .arg(QString::number(model->stats_getTotalServerEarn(), 'f', 2)))
                                   , 7, 1);
    layoutDetailedStats->addWidget(new QLabel(QString("%1 €")
                                              .arg(QString::number(model->stats_getGuadagnoNetto(DefaultKWHCost), 'f', 2)))
                                   , 8, 1);


    layoutGeneralPage->addLayout(layoutOwo);
}

void MainWindow::updateSectionTitle()
{
    int i = tabs->currentIndex();

    if(i == 0)
        sectionInfoTitle->setText("Bentornato utente! <br>"
                                  "Nella <b>dashboard</b> è possibile visualizzare le informazioni principali, "
                                  "gli avvisi e le statistiche aziendali.");
    else if(i == 1)
        sectionInfoTitle->setText("In <b>gestione server</b> è possibile gestire i servizi attivi, "
                                  "aggiungerne di nuovi, editare, cancellare e ricercare per tipologia. "
                                  "<br><b>Per visionare più dettagli, clicca su un server della tabella.</b> Usa CTRL+K (CMD+K) per chiudere i dettagli.");
    else if(i == 2)
        sectionInfoTitle->setText("In <b>gestione nodi</b> si possono visualizzare i nodi hardware attivi "
                                  "con le risorse e i costi di mantenimento. "
                                  "<br><b>Per visionare più dettagli, clicca su un nodo della tabella.</b>  Usa CTRL+K (CMD+K) per chiudere i dettagli.");
}

void MainWindow::UpdateServer()
{
    // In ordine:
    // 1) Aggiorno le mappe di statistica
    // 2) Aggiorno la riga corrente
    // 3) Ribuildo la pagina generale con le nuove statistiche (in background)
    // 4) Aggiorno le notifiche

    model->stats_update();
    serverAdapter->currentRowToUpdate(getServerSelection());
    updateGeneralPage();
    updateNotification();
    setFileNotification("[!] Modifiche non salvate");
}

void MainWindow::DeleteServer()
{
    // Richiedo conferma per la cancellazione

    if(!showServerDeleteConfirm())
        return;

    // Aggiorno i dati e chiudo i dettagli

    serverProxy->removeRow(serverTable->currentIndex().row()); // Cancellazione delegata all'adapter
    detailAutoCleaner();
    model->stats_update();
    updateGeneralPage();
    updateNotification();
    setFileNotification("[!] Modifiche non salvate");
}

void MainWindow::AddServer()
{
    model->stats_update();
    serverAdapter->insertRow(0);
    updateGeneralPage();
    updateNotification();
    setFileNotification("[!] Modifiche non salvate");
}

void MainWindow::DeleteServerResults()
{
    if(!showServerDeleteAllConfirm())
        return;

    detailAutoCleaner();
    serverProxy->removeServerResults();
    model->stats_update();
    updateGeneralPage();
    updateNotification();
    setFileNotification("[!] Modifiche non salvate");
}

void MainWindow::UpdateSearchFilter()
{
    QRegExp regex(fSearch->text(), Qt::CaseInsensitive, QRegExp::Wildcard);
    serverProxy->setFilterRegExp(regex);
}

void MainWindow::showInfoBox() const
{
    QMessageBox box(QMessageBox::Information, "Informazioni applicazione",
                    QString("Software realizzato da <u>%1</u> <br>"
                    "Progetto di Programmazione a Oggetti, "
                    "UniPD 2019 <br><br>"
                    "Le <u>icone</u> (in .SVG e .PNG) usate in questo progetto sono rilasciate"
                    " su licenza (<a href='https://fontawesome.com/license/free'>Creative and Commons By 4.0</a>)"
                    " da parte di FontAwesome. "
                    "Alcune icone sono state modificate appositamente per le colorazioni "
                    "e le dimensioni.").arg(model->getCredits()), QMessageBox::Ok);
    box.exec();
}

void MainWindow::showFileInfoBox() const
{
    QString formattedFileInfo;

    if(model->getFilepath().isEmpty())
        formattedFileInfo = QString("Nessun file caricato al momento.");
    else
    {
        formattedFileInfo =
            QString("<u>Nome file:</u> %1 <br>"
                    "<u>Grandezza file:</u> %2 KB <br>"
                    "<u>Ultima modifica:</u> %3 <br>")
                .arg(QFileInfo(model->getFilepath()).fileName())
                .arg(QString::number(
                         static_cast<double>(QFileInfo(model->getFilepath()).size()) / 1024.0, 'f', 2))
                .arg(QFileInfo(model->getFilepath()).lastModified().toString("dd/MM/yyyy - hh:mm:ss"));
    }

    QMessageBox box(QMessageBox::Information,
                    "Informazioni file",
                    formattedFileInfo,
                    QMessageBox::Ok);
    box.exec();
}

void MainWindow::cleanLayout(QLayout* layout)
{
    // La funzione richiama ricorsivamente la cancellazione dei layout
    // e dei widget figli presenti

    QLayoutItem * child;
    while(layout->count()!=0)
    {
        child = layout->takeAt(0);

        if(child->layout() != nullptr)
            cleanLayout(child->layout());
        else if(child->widget() != nullptr)
            delete child->widget();

        delete child;
    }
}

void MainWindow::detailAutoCleaner()
{
    if(viewInfo)
    {
        cleanLayout(viewInfo);
        nodesTable->clearSelection();
        serverTable->clearSelection();
        fSearch->setEnabled(true);
        fSelect->setEnabled(true);
    }
}
