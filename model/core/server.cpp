#include "server.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

std::map<QString, Server *> * Server::itemsMap = nullptr;

Server::Server(char no, QString lab, QString ip, unsigned short core, double ram, unsigned short disk, double BASE, double SPEED)
    : Node(no),
      Label(lab),
      IP(ip),
      BasePrice(BASE),
      MaxCPUSpeed(SPEED)
{
    // Controlli di sanità
    setCores(core);
    setRam(ram);
    setDisk(disk);
}

char Server::getNode() const
{ return Node; }

QString Server::getLabel() const
{ return Label; }

QString Server::getIP() const
{ return IP; }

unsigned short Server::getCores() const
{ return Cores; }

double Server::getRam() const
{ return Ram; }

unsigned short Server::getDisk() const
{ return Disk; }

void Server::setNode(char n)
{ if(Node != n) Node = n; }

void Server::setLabel(QString l)
{ if(Label != l) Label = l; }

void Server::setIP(QString i)
{ if(IP != i) IP = i; }

void Server::setCores(unsigned short c)
{ if(Cores != c) Cores = (c < 1) ? 1 : c; }

void Server::setRam(double r)
{ if(Ram != r) Ram = (r < 0.5) ? 0.5 : r; }

void Server::setDisk(unsigned short d)
{ if(Disk != d) Disk = (d < 20) ? 20 : d; }

unsigned short Server::multiplierCores() const
{ return 1; }

double Server::multiplierRam() const
{ return 0.5; }

unsigned short Server::multiplierDisk() const
{ return 10; }

double Server::baseSpecsPrice(double perCore, double perRam, double perDisk) const
{
    double total = BasePrice;
    total += (Cores > 1 ? (Cores - 1) * perCore : 0.0);
    total += (Ram > 0.5 ? ((Ram - 0.5) / 0.5) * perRam : 0.0);
    total += (Disk > 20 ? ((Disk - 20) / 10) * perDisk : 0.0);
    return total;
}

Server* Server::dataUnserialize(QXmlStreamReader & reader)
{
    if(reader.name() != "server")
        throw std::string("'server' non è stato trovato nel file.");

    QString tipo = reader.attributes().value("type").toString();

    if(itemsMap->count(tipo) == 0)
        throw std::string("'type' non è stato trovato come attributo.");

    return (*itemsMap)[tipo]->createObjectFromData(reader);
}

const std::map<QString, Server *> * Server::staticMap()
{
    return const_cast<std::map<QString, Server *>*>(itemsMap);
}

bool Server::operator==(const Server & s) const
{
    return Label == s.Label && IP == s.IP && Cores == s.Cores && Ram == s.Ram && Disk == s.Disk
            && BasePrice == s.BasePrice && MaxCPUSpeed == s.MaxCPUSpeed;
}

bool Server::operator!=(const Server & s) const
{ return !(*this == s); }






