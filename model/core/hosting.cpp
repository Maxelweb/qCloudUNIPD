#include "hosting.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

// Builder del tipo della classe

StaticTypeBuilder<Hosting> Hosting::initType;


// Gestione dati

Hosting * Hosting::createObjectFromData(QXmlStreamReader & r) const
{
    char node = '0';
    QString lab = "none", ip = "0.0.0.0";
    unsigned short core = 1, disk = 20;
    double ram = 0.5;
    bool ssh = false, boost = false;

    if(r.readNextStartElement() && r.name() == "node")
            node = r.readElementText().at(0).toLatin1();
    if(r.readNextStartElement() && r.name() == "name")
            lab = r.readElementText();
    if(r.readNextStartElement() && r.name() == "ip")
            ip = r.readElementText();
    if(r.readNextStartElement() && r.name() == "core")
            core = static_cast<unsigned short>(r.readElementText().toShort());
    if(r.readNextStartElement() && r.name() == "ram")
            ram = r.readElementText().toDouble();
    if(r.readNextStartElement() && r.name() == "disk")
            disk = static_cast<unsigned short>(r.readElementText().toShort());
    if(r.readNextStartElement() && r.name() == "sshaccess")
            ssh = r.readElementText() == "yes" ? true : false;
    if(r.readNextStartElement() && r.name() == "turboboost")
            boost = r.readElementText() == "yes" ? true : false;

    r.skipCurrentElement();
    return new Hosting(node, lab, ip, core, ram, disk, ssh, boost);
}


void Hosting::dataSerialize(QXmlStreamWriter & w) const
{
    w.writeStartElement("server");
        w.writeAttribute("type", staticType());

        w.writeStartElement("node");
        w.writeCharacters(QString(getNode()));
        w.writeEndElement(); // ---
        w.writeStartElement("name");
        w.writeCharacters(getLabel());
        w.writeEndElement(); // ---
        w.writeStartElement("ip");
        w.writeCharacters(getIP());
        w.writeEndElement(); // ---
        w.writeStartElement("core");
        w.writeCharacters(QString::number(getCores()));
        w.writeEndElement(); // ---
        w.writeStartElement("ram");
        w.writeCharacters(QString::number(getRam(), 'f', 1));
        w.writeEndElement(); // ---
        w.writeStartElement("disk");
        w.writeCharacters(QString::number(getDisk()));
        w.writeEndElement(); // ---
        w.writeStartElement("sshaccess");
        w.writeCharacters(SSHAccess ? "yes" : "no");
        w.writeEndElement(); // ---
        w.writeStartElement("turboboost");
        w.writeCharacters(TurboBoost ? "yes" : "no");
        w.writeEndElement(); // ---

    w.writeEndElement();

    if (w.hasError())
        throw "Errore in scrittura (H)";
}


QString Hosting::staticType() const
{ return QString("Hosting"); }


// Costruttori e metodi

Hosting::Hosting(char no, QString lab, QString ip,
                 unsigned short core, double ram, unsigned short disk,
                 bool ssh, bool boost)
    : Server(no, lab, ip, core, ram, disk, Hosting_BasePrice, Hosting_MaxSpeed),
      SSHAccess(ssh),
      TurboBoost(boost)
{ }

bool Hosting::hasSSHAccess() const
{ return SSHAccess; }

bool Hosting::hasTurboBoost() const
{ return TurboBoost; }

void Hosting::setSSHAccess(bool s)
{ if(SSHAccess != s) SSHAccess = s; }

void Hosting::setTurboBoost(bool t)
{ if(TurboBoost != t) TurboBoost = t; }

double Hosting::calculatePrice() const
{
    double total = baseSpecsPrice();
    total += SSHAccess ? Hosting_BasePrice_SSHAccess : 0.0;
    total += TurboBoost ? Hosting_BasePrice_Turbo : 0.0;
    return total;
}

double Hosting::serverSpeed() const
{
    return (TurboBoost ? MaxCPUSpeed + 0.25*MaxCPUSpeed : MaxCPUSpeed);
}

unsigned short Hosting::resourcesWeight() const
{
    return Hosting_Weight;
}

Hosting* Hosting::deepClone() const
{ return new Hosting(*this); }


unsigned short Hosting::minCores() const
{ return 1; }

unsigned short Hosting::maxCores() const
{ return 1; }

double Hosting::minRam() const
{ return 0.5; }

double Hosting::maxRam() const
{ return 1.0; }

unsigned short Hosting::minDisk() const
{ return 20; }

unsigned short Hosting::maxDisk() const
{ return 100; }

bool Hosting::onlyVirtualized() const
{ return true; }

bool Hosting::operator ==(const Server & s) const
{
    if(!(dynamic_cast<const Hosting *>(&s)))
        return false;

    return Server::operator ==(s)
            && dynamic_cast<const Hosting &>(s).hasSSHAccess() == SSHAccess
            && dynamic_cast<const Hosting &>(s).hasTurboBoost() == TurboBoost;
}

bool Hosting::operator !=(const Server & s) const
{ return !(*this == s); }
