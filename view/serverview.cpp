#include "serverview.h"

#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include "../model/model.h"
#include "../model/core/server.h"
#include "../model/core/vps.h"
#include "../model/core/dedicated.h"
#include "../model/core/hosting.h"
#include "mainwindow.h"

QRegExpValidator *ServerView::create_ipValidator() const
{
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange  + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex);
    return ipValidator;
}

void ServerView::update_addServerTemplate(QString currentType)
{
    QGridLayout * grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(5);

    // Controllo di sanità
    if(Server::staticMap()->find(currentType) == Server::staticMap()->end()
            || currentType.isEmpty())
        server = Server::staticMap()->begin()->second;
    else
        server = Server::staticMap()->find(currentType)->second;

    // Generazione input
    cores = new QComboBox();
    for(unsigned short i = server->minCores(); i <= server->maxCores(); i+=server->multiplierCores())
        cores->addItem(QString::number(i), i);

    ram = new QComboBox();
    for(double i = server->minRam(); i <= server->maxRam(); i+=server->multiplierRam())
        ram->addItem(QString::number(i,'f',1), i);

    disk = new QComboBox();
    for(unsigned short i = server->minDisk(); i <= server->maxDisk(); i+=server->multiplierDisk())
        disk->addItem(QString::number(i), i);


    grid->addWidget(new QLabel("<b>vCore:</b>"), 0, 0);
    grid->addWidget(new QLabel("<b>RAM (GB):</b>"), 1, 0);
    grid->addWidget(new QLabel("<b>Disco (GB):</b>"), 2, 0);

    grid->addWidget(cores, 0, 1);
    grid->addWidget(ram, 1, 1);
    grid->addWidget(disk, 2, 1);

    if(currentType == "VPS")
    {
        vpsSNAP = new QSpinBox();
        vpsSNAP->setRange(0, 10);
        vpsDDOS = new QCheckBox("Abilitata");
        grid->addWidget(new QLabel(QString("<b>Snapshots:</b>")), 3, 0);
        grid->addWidget(new QLabel(QString("<b>Protezione anti-ddos:</b>")), 4, 0);
        grid->addWidget(vpsSNAP, 3, 1);
        grid->addWidget(vpsDDOS, 4, 1);
    }
    else if(currentType == "Dedicated")
    {
        dedIP = new QSpinBox();
        dedIP->setRange(0, 10);
        dedBAK = new QCheckBox("Abilitato");
        grid->addWidget(new QLabel(QString("<b>Backup attivo:</b>")), 3, 0);
        grid->addWidget(new QLabel(QString("<b>Nr. IP aggiuntivi:</b>")), 4, 0);
        grid->addWidget(dedBAK, 3, 1);
        grid->addWidget(dedIP, 4, 1);
    }
    else if(currentType == "Hosting")
    {
        hostSSH = new QCheckBox("Abilitato");
        hostBOOST = new QCheckBox("Abilitato");
        grid->addWidget(new QLabel(QString("<b>Accesso SSH attivo:</b>")), 3, 0);
        grid->addWidget(new QLabel(QString("<b>Site Turbo Boost:</b>")), 4, 0);
        grid->addWidget(hostSSH, 3, 1);
        grid->addWidget(hostBOOST, 4, 1);
    }

    dynamicLayout->addLayout(grid);
}

void ServerView::update_currentServer()
{
    server->setLabel(etichetta->text());
    server->setIP(ip->text());
    server->setNode(nodes->currentText().at(0).toLatin1());
    server->setCores(cores->currentText().toUShort());
    server->setRam(ram->currentText().toDouble());
    server->setDisk(disk->currentText().toUShort());

    if(server->staticType() == "VPS")
    {
        static_cast<VPS*>(server)->setSnapshots(vpsSNAP->text().toUShort());
        static_cast<VPS*>(server)->setAntiDDoS(vpsDDOS->isChecked());
    }
    else if(server->staticType() == "Dedicated")
    {
        static_cast<Dedicated*>(server)->setExtraIps(dedIP->text().toUShort());
        static_cast<Dedicated*>(server)->setBackup(dedBAK->isChecked());
    }
    else if(server->staticType() == "Hosting")
    {
        static_cast<Hosting*>(server)->setSSHAccess(hostSSH->isChecked());
        static_cast<Hosting*>(server)->setTurboBoost(hostBOOST->isChecked());
    }

    model->setSaved(false);
}


ServerView::ServerView(Model* m, Server* s, QWidget *parent)
    : QWidget(parent),
      model(m),
      server(s),
      error(new QLabel()),
      etichetta(nullptr),
      tipo(nullptr),
      ip(nullptr),
      nodes(nullptr),
      cores(nullptr),
      ram(nullptr),
      disk(nullptr),
      vpsSNAP(nullptr),
      vpsDDOS(nullptr),
      dedIP(nullptr),
      dedBAK(nullptr),
      hostSSH(nullptr),
      hostBOOST(nullptr),
      dynamicLayout(nullptr)
{
     error->setWordWrap(true);
     error->setVisible(false);
}

QLayout *ServerView::readServerDetail()
{
    QGridLayout * grid = new QGridLayout();

    grid->addWidget(new QLabel("<b>Tipologia:</b>"), 0, 0);
    grid->addWidget(new QLabel("<b>Indirizzo IP:</b>"), 1, 0);
    grid->addWidget(new QLabel("<b>Nodo:</b>"), 2, 0);
    grid->addWidget(new QLabel("<b>vCore:</b>"), 3, 0);
    grid->addWidget(new QLabel("<b>RAM (GB):</b>"), 4, 0);
    grid->addWidget(new QLabel("<b>Disco (GB):</b>"), 5, 0);
    grid->addWidget(new QLabel("<b>Velocità CPU (Ghz):</b>"), 8, 0);
    grid->addWidget(new QLabel("<b>Costo mensile (EUR):</b>"), 9, 0);

    grid->addWidget(new QLabel(server->staticType()), 0, 1);
    grid->addWidget(new QLabel(server->getIP()), 1, 1);
    grid->addWidget(new QLabel(QString(server->getNode())), 2, 1);
    grid->addWidget(new QLabel(QString::number(server->getCores())), 3, 1);
    grid->addWidget(new QLabel(QString::number(server->getRam(), 'f', 1)), 4, 1);
    grid->addWidget(new QLabel(QString::number(server->getDisk())), 5, 1);
    grid->addWidget(new QLabel(QString::number(server->serverSpeed(), 'f', 2)), 8, 1);
    grid->addWidget(new QLabel(QString::number(server->calculatePrice(), 'f', 2)), 9, 1);

    if(server->staticType() == "VPS")
    {
        grid->addWidget(new QLabel(QString("<b>Snapshots:</b>")), 6, 0);
        grid->addWidget(new QLabel(QString("<b>Protezione anti-ddos:</b>")), 7, 0);
        grid->addWidget(new QLabel(QString::number((static_cast<VPS*>(server))->getSnapshots())), 6, 1);
        grid->addWidget(new QLabel((static_cast<VPS*>(server))->hasAntiDDoS() ? "Sì" : "No"), 7, 1);
    }
    else if(server->staticType() == "Dedicated")
    {
        grid->addWidget(new QLabel(QString("<b>Backup attivo:</b>")), 6, 0);
        grid->addWidget(new QLabel(QString("<b>Nr. IP aggiuntivi:</b>")), 7, 0);
        grid->addWidget(new QLabel((static_cast<Dedicated*>(server))->hasBackup() ? "Sì" : "No"), 6, 1);
        grid->addWidget(new QLabel(QString::number((static_cast<Dedicated*>(server))->getExtraIps())), 7, 1);
    }
    else if(server->staticType() == "Hosting")
    {
        grid->addWidget(new QLabel(QString("<b>Accesso SSH attivo:</b>")), 6, 0);
        grid->addWidget(new QLabel(QString("<b>Site Turbo Boost:</b>")), 7, 0);
        grid->addWidget(new QLabel((static_cast<Hosting*>(server))->hasSSHAccess() ? "Sì" : "No"), 6, 1);
        grid->addWidget(new QLabel((static_cast<Hosting*>(server))->hasTurboBoost() ? "Sì" : "No"), 7, 1);
    }


    return grid;
}

QLayout *ServerView::editServerDetail()
{

    QVBoxLayout * master = new QVBoxLayout();
    master->setMargin(0);

    // Label Controllo errori

    master->addWidget(error);

    // Generazione input boxes

    etichetta = new QLineEdit(server->getLabel());
    etichetta->setMaxLength(32);


    tipo = new QComboBox();
    for(auto it = Server::staticMap()->begin(); it != Server::staticMap()->end(); ++it)
        tipo->addItem((*it).first);
    tipo->setCurrentText(server->staticType());
    tipo->setEnabled(false);


    ip = new QLineEdit();
    ip->setValidator(create_ipValidator());
    ip->setText(server->getIP());


    nodes = new QComboBox();
    auto nodesVector = model->getNodesIdList();
    for(auto it = nodesVector.begin(); it != nodesVector.end(); ++it)
        nodes->addItem(QString(*it));
    nodes->setCurrentText(QString(server->getNode()));

    cores = new QComboBox();
    for(unsigned short i = server->minCores(); i <= server->maxCores(); i+=server->multiplierCores())
        cores->addItem(QString::number(i), i);
    cores->setCurrentText(QString::number(server->getCores()));

    ram = new QComboBox();
    for(double i = server->minRam(); i <= server->maxRam(); i+=server->multiplierRam())
        ram->addItem(QString::number(i,'f',1), i);
    ram->setCurrentText(QString::number(server->getRam(), 'f', 1));

    disk = new QComboBox();
    for(unsigned short i = server->minDisk(); i <= server->maxDisk(); i+=server->multiplierDisk())
        disk->addItem(QString::number(i), i);
    disk->setCurrentText(QString::number(server->getDisk()));

    // Generazione griglia form

    QGridLayout * grid = new QGridLayout();
    grid->setSpacing(8);

    grid->addWidget(new QLabel("<b>Etichetta:</b>"), 0, 0);
    grid->addWidget(new QLabel("<b>Tipologia:</b>"), 1, 0);
    grid->addWidget(new QLabel("<b>Indirizzo IP:</b>"), 2, 0);
    grid->addWidget(new QLabel("<b>Nodo:</b>"), 3, 0);
    grid->addWidget(new QLabel("<b>vCore:</b>"), 4, 0);
    grid->addWidget(new QLabel("<b>RAM (GB):</b>"), 5, 0);
    grid->addWidget(new QLabel("<b>Disco (GB):</b>"), 6, 0);

    grid->addWidget(etichetta, 0, 1);
    grid->addWidget(tipo, 1, 1);
    grid->addWidget(ip, 2, 1);
    grid->addWidget(nodes, 3, 1);
    grid->addWidget(cores, 4, 1);
    grid->addWidget(ram, 5, 1);
    grid->addWidget(disk, 6, 1);

    if(server->staticType() == "VPS")
    {
        vpsSNAP = new QSpinBox();
        vpsSNAP->setRange(0, 10);
        vpsSNAP->setValue(static_cast<int>(static_cast<VPS*>(server)->getSnapshots()));

        vpsDDOS = new QCheckBox("Abilitata");
        vpsDDOS->setChecked(static_cast<VPS*>(server)->hasAntiDDoS() ? true : false);

        grid->addWidget(new QLabel(QString("<b>Snapshots:</b>")), 7, 0);
        grid->addWidget(new QLabel(QString("<b>Protezione anti-ddos:</b>")), 8, 0);
        grid->addWidget(vpsSNAP, 7, 1);
        grid->addWidget(vpsDDOS, 8, 1);
    }
    else if(server->staticType() == "Dedicated")
    {
        dedIP = new QSpinBox();
        dedIP->setRange(0, 10);
        dedIP->setValue(static_cast<int>(static_cast<Dedicated*>(server)->getExtraIps()));

        dedBAK = new QCheckBox("Abilitato");
        dedBAK->setChecked(static_cast<Dedicated*>(server)->hasBackup() ? true : false);

        grid->addWidget(new QLabel(QString("<b>Backup attivo:</b>")), 7, 0);
        grid->addWidget(new QLabel(QString("<b>Nr. IP aggiuntivi:</b>")), 8, 0);
        grid->addWidget(dedBAK, 7, 1);
        grid->addWidget(dedIP, 8, 1);
    }
    else if(server->staticType() == "Hosting")
    {
        hostSSH = new QCheckBox("Abilitato");
        hostSSH->setChecked(static_cast<Hosting*>(server)->hasSSHAccess() ? true : false);

        hostBOOST = new QCheckBox("Abilitato");
        hostBOOST->setChecked(static_cast<Hosting*>(server)->hasTurboBoost() ? true : false);

        grid->addWidget(new QLabel(QString("<b>Accesso SSH attivo:</b>")), 7, 0);
        grid->addWidget(new QLabel(QString("<b>Site Turbo Boost:</b>")), 8, 0);
        grid->addWidget(hostSSH, 7, 1);
        grid->addWidget(hostBOOST, 8, 1);
    }

    QPushButton * saveButton = new QPushButton(QIcon(":res/styles/icons/check.svg"),"Esegui modifiche");

    grid->addWidget(saveButton, 9, 0, 1, 2, Qt::AlignRight);

    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
    grid->addWidget(empty);


    // Connessione
    connect(saveButton, SIGNAL(clicked()), this, SLOT(editServerDetail_verify()));

    // Conclusione
    master->addLayout(grid);

    return master;
}

QLayout *ServerView::addServerDetail()
{
    // Generazione layout base
    QVBoxLayout * master = new QVBoxLayout();
    master->setMargin(0);

    // Label Controllo errori
    master->addWidget(error);

    // Generazione input boxes
    etichetta = new QLineEdit();
    etichetta->setMaxLength(32);

    tipo = new QComboBox();
    for(auto it = Server::staticMap()->begin(); it != Server::staticMap()->end(); ++it)
        tipo->addItem((*it).first);

    ip = new QLineEdit();
    ip->setValidator(create_ipValidator());

    nodes = new QComboBox();
    auto nodesVector = model->getNodesIdList();
    for(auto it = nodesVector.begin(); it != nodesVector.end(); ++it)
        nodes->addItem(QString(*it));
    nodes->setCurrentText(QString(model->stats_getLessUsedNode()));

    // Inizializzo il layout dinamico
    dynamicLayout = new QHBoxLayout();

    // Generazione griglia form
    QGridLayout * grid = new QGridLayout();
    grid->setSpacing(5);

    grid->addWidget(new QLabel("<b>Tipologia:</b>"), 0, 0);
    grid->addWidget(new QLabel("<b>Etichetta:</b>"), 1, 0);
    grid->addWidget(new QLabel("<b>Indirizzo IP:</b>"), 2, 0);
    grid->addWidget(new QLabel("<b>Nodo:</b>"), 3, 0);

    grid->addWidget(tipo, 0, 1);
    grid->addWidget(etichetta, 1, 1);
    grid->addWidget(ip, 2, 1);
    grid->addWidget(nodes, 3, 1);

    master->addLayout(grid);

    update_addServerTemplate(tipo->currentText());

    master->addLayout(dynamicLayout);

    // Bottone di conferma
    QPushButton * saveButton = new QPushButton(QIcon(":res/styles/icons/check.svg"),"Conferma aggiunta");
    master->addWidget(saveButton, 1, Qt::AlignRight);

    // Spazio
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
    master->addWidget(empty);

    // Connessione
    connect(saveButton, SIGNAL(clicked()), this, SLOT(addServerDetail_verify()));
    connect(tipo, SIGNAL(currentIndexChanged(int)), this, SLOT(currentTypeChanged()));

    return master;
}

bool ServerView::editServerDetail_verify()
{
    if(!serverInput_verify())
        return false;

    update_currentServer();
    emit pokeUpdateServer();
    return true;
}

bool ServerView::addServerDetail_verify()
{
    if(!serverInput_verify())
        return false;

    Server * nuovoServer = server->deepClone();
    Server * oldServer = server;
    server = nuovoServer;
    update_currentServer();
    model->addServer(server);
    server = oldServer;
    etichetta->clear();
    ip->clear();
    emit pokeAddServer();
    return true;
}

bool ServerView::serverInput_verify()
{
    error->setVisible(true);
    error->setProperty("class", "error-box");
    error->style()->unpolish(error);
    error->style()->polish(error);

    if(etichetta->text().isEmpty())
    {
        error->setText("<b>Errore:</b> l'etichetta non può essere vuota!");
        return false;
    }

    if(ip->text().isEmpty())
    {
        error->setText("<b>Errore:</b> il campo IP è vuoto.");
        return false;
    }

    if(!ip->hasAcceptableInput())
    {
        error->setText("<b>Errore:</b> il formato del IP è invalido. Es: <u>192.168.0.1</u>");
        return false;
    }


    if(server->getLabel() != etichetta->text())
        if(model->labelChecker(etichetta->text()))
        {
            error->setText("<b>Errore:</b> l'etichetta è già stata usata per un altro server.");
            return false;
        }

    if(server->getIP() != ip->text())
        if(model->ipChecker(ip->text()))
        {
            error->setText("<b>Errore:</b> l'indirizzo IP è già stato usato per un altro server.");
            return false;
        }

    if(server->onlyVirtualized() && !model->getNodeById(nodes->currentText().at(0).toLatin1())->isVirtualized())
    {
        error->setText(QString("<b>Errore:</b> il server richiede un nodo <u>virtualizzato</u> "
                               "(il nodo <u>%1</u> è <u>dedicato</u>).").arg(nodes->currentText()));
        return false;
    }

    error->setProperty("class", "success-box");
    error->style()->unpolish(error);
    error->style()->polish(error);
    error->setText("<b>Operazione eseguita</b> con successo!");

    return true;
}

void ServerView::currentTypeChanged()
{
    if(tipo && dynamicLayout)
    {
        MainWindow::cleanLayout(dynamicLayout);
        update_addServerTemplate(tipo->currentText());
    }
}
