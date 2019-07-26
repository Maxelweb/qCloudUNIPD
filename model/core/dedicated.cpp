#include "dedicated.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

// Builder del tipo della classe

StaticTypeBuilder<Dedicated> Dedicated::initType;

// Gestione dati

Dedicated * Dedicated::createObjectFromData(QXmlStreamReader & r) const
{
    char node = '0';
    QString lab = "none-dedicated", ip = "0.0.0.0";
    unsigned short core = 8, disk = 240, ips = 0;
    double ram = 4;
    bool bak = false;

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
    if(r.readNextStartElement() && r.name() == "extraips")
            ips = static_cast<unsigned short>(r.readElementText().toShort());
    if(r.readNextStartElement() && r.name() == "backup")
            bak = r.readElementText() == "yes" ? true : false;

    r.skipCurrentElement();
    return new Dedicated(node, lab, ip, core, ram, disk, bak, ips);
}


void Dedicated::dataSerialize(QXmlStreamWriter & w) const
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
        w.writeStartElement("extraips");
        w.writeCharacters(QString::number(ExtraIps));
        w.writeEndElement(); // ---
        w.writeStartElement("backup");
        w.writeCharacters(Backup ? "yes" : "no");
        w.writeEndElement(); // ---

    w.writeEndElement();

    if (w.hasError())
        throw "Errore in scrittura (D)";
}

QString Dedicated::staticType() const
{ return QString("Dedicated"); }


// Costruttori e metodi

Dedicated::Dedicated(char no, QString lab, QString ip,
                     unsigned short core, double ram, unsigned short disk,
                     bool bak, unsigned short ips)
    : Server(no, lab, ip, core, ram, disk, Dedicated_BasePrice, Dedicated_MaxSpeed),
      Backup(bak),
      ExtraIps(ips)
{ }

bool Dedicated::hasBackup() const
{ return Backup; }

unsigned short Dedicated::getExtraIps() const
{ return ExtraIps; }

void Dedicated::setBackup(bool b)
{ if(Backup != b) Backup = b; }

void Dedicated::setExtraIps(unsigned short e)
{ if(ExtraIps != e) ExtraIps = e; }

double Dedicated::baseSpecsPrice(double perCore, double perRam, double perDisk) const
{
    double total = BasePrice;
    total += (getCores() > 8 ? (getCores() - 8) * perCore : 0.0);
    total += (getRam() > 4 ? ((getRam() - 4) / 4) * perRam : 0.0);
    total += (getDisk() > 240 ? ((getDisk() - 240) / 240) * perDisk : 0.0);
    return total;
}

double Dedicated::calculatePrice() const
{
    double total = Dedicated::baseSpecsPrice();
    total += Backup ? Dedicated_BasePrice_Backup : 0.0;
    total += ExtraIps * Dedicated_BasePrice_PerIp;
    return total;
}

double Dedicated::serverSpeed() const
{
    return MaxCPUSpeed;
}

unsigned short Dedicated::resourcesWeight() const
{
    return Dedicated_Weight;
}

Dedicated* Dedicated::deepClone() const
{ return new Dedicated(*this); }


unsigned short Dedicated::minCores() const
{ return 8; }

unsigned short Dedicated::maxCores() const
{ return 16; }

double Dedicated::minRam() const
{ return 4.0; }

double Dedicated::maxRam() const
{ return 16.0; }

unsigned short Dedicated::minDisk() const
{ return 240; }

unsigned short Dedicated::maxDisk() const
{ return 480; }

unsigned short Dedicated::multiplierCores() const
{ return 4; }

double Dedicated::multiplierRam() const
{ return 4; }

unsigned short Dedicated::multiplierDisk() const
{ return 240; }

bool Dedicated::onlyVirtualized() const
{ return false; }

bool Dedicated::operator ==(const Server & s) const
{
    if(!(dynamic_cast<const Dedicated *>(&s)))
        return false;

    return Server::operator ==(s)
            && dynamic_cast<const Dedicated&>(s).hasBackup() == Backup
            && dynamic_cast<const Dedicated&>(s).getExtraIps() == ExtraIps;
}

bool Dedicated::operator !=(const Server & s) const
{ return !(*this == s); }
