#include "vps.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

// Builder del tipo della classe

StaticTypeBuilder<VPS> VPS::initType;


// Gestione dati

VPS * VPS::createObjectFromData(QXmlStreamReader & r) const
{
    char node = '0';
    QString lab = "none-vps", ip = "0.0.0.0";
    unsigned short core = 1, disk = 20, snap = 0;
    double ram = 0.5;
    bool ddos = false;

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
    if(r.readNextStartElement() && r.name() == "snapshots")
            snap = static_cast<unsigned short>(r.readElementText().toShort());
    if(r.readNextStartElement() && r.name() == "antiddos")
            ddos = r.readElementText() == "yes" ? true : false;

    r.skipCurrentElement();
    return new VPS(node, lab, ip, core, ram, disk, snap, ddos);
}

void VPS::dataSerialize(QXmlStreamWriter & w) const
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
        w.writeStartElement("snapshots");
        w.writeCharacters(QString::number(Snapshots));
        w.writeEndElement(); // ---
        w.writeStartElement("antiddos");
        w.writeCharacters(AntiDDoS ? "yes" : "no");
        w.writeEndElement(); // ---

    w.writeEndElement();

    if (w.hasError())
        throw "Errore in scrittura (V)";
}


// Metodi e costruttori di classe

VPS::VPS(char no, QString lab, QString ip,
         unsigned short core, double ram, unsigned short disk,
         unsigned short snap, bool ddos)
   : Server(no, lab, ip, core, ram, disk, VPS_BasePrice, VPS_MaxSpeed),
     Snapshots(snap),
     AntiDDoS(ddos)
{ }

unsigned short VPS::getSnapshots() const
{ return Snapshots; }

bool VPS::hasAntiDDoS() const
{ return AntiDDoS; }

void VPS::setSnapshots(unsigned short s)
{ if(Snapshots != s) Snapshots = s; }

void VPS::setAntiDDoS(bool a)
{ if(AntiDDoS != a) AntiDDoS = a; }

double VPS::calculatePrice() const
{
    double total = baseSpecsPrice();
    total += Snapshots * VPS_BasePrice_PerSnapshots;
    total += (AntiDDoS ? VPS_BasePrice_AntiDDoS : 0.0);
    return total;
}

double VPS::serverSpeed() const
{
    return (AntiDDoS ? MaxCPUSpeed*0.95 : MaxCPUSpeed);
}

unsigned short VPS::resourcesWeight() const
{
    if(getCores() > 4) return VPS_Weight+2;
    else if(getCores() > 2) return VPS_Weight+1;
    else return VPS_Weight;
}

VPS* VPS::deepClone() const
{ return new VPS(*this); }


QString VPS::staticType() const
{ return QString("VPS"); }

unsigned short VPS::minCores() const
{ return 1; }

unsigned short VPS::maxCores() const
{ return 8; }

double VPS::minRam() const
{ return 0.5; }

double VPS::maxRam() const
{ return 8.0; }

unsigned short VPS::minDisk() const
{ return 20; }

unsigned short VPS::maxDisk() const
{ return 200; }

bool VPS::onlyVirtualized() const
{ return true; }

bool VPS::operator ==(const Server & s) const
{
    if(!(dynamic_cast<const VPS *>(&s)))
        return false;

    return Server::operator ==(s)
            && dynamic_cast<const VPS &>(s).getSnapshots() == Snapshots
            && dynamic_cast<const VPS &>(s).hasAntiDDoS() == AntiDDoS;
}

bool VPS::operator !=(const Server & s) const
{ return !(*this == s); }

