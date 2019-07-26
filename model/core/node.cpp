#include "node.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>


Node::Node(char letter, QString name, unsigned short res, bool virt)
    : Id(letter),
      Name(name),
      ResourcesLimit(res),
      Virtualized(virt)
{ }

char Node::getId() const
{ return Id; }

QString Node::getName() const
{ return Name; }

unsigned short Node::getLimits() const
{ return ResourcesLimit; }

bool Node::isVirtualized() const
{ return Virtualized; }


// Si ipotizza che ciascuna macchina fisica consumi 250 watt come massimo.
// Una macchina fisica è identificata da 1 a 4 risorse

double Node::getConsumo(bool idle) const
{ return static_cast<double>((ResourcesLimit/4 + 1) * 250) * (idle ? 0.25 : 1.0); }

// Calcolo il costo mensile per kilowatt/ora

double Node::getCosto(double costokwh, bool idle) const
{ return (getConsumo(idle) / 1000.0) * (costokwh * 24.0 * 30.0); }

// Gestione dati

Node* Node::dataUnserialize(QXmlStreamReader & r)
{
    if(r.name() != "node")
        throw std::string("'node' non è stato trovato nel file dei dati.");

    char node = '0';
    QString name = "Undefined Node";
    unsigned short res = 0;
    bool virt = false;

    if(r.readNextStartElement() && r.name() == "id") 
            node = r.readElementText().at(0).toLatin1();

    if(r.readNextStartElement() && r.name() == "name")
            name = r.readElementText();

    if(r.readNextStartElement() && r.name() == "limits")
            res = static_cast<unsigned short>(r.readElementText().toShort());

    if(r.readNextStartElement() && r.name() == "virtualized")
            virt = r.readElementText() == "yes" ? true : false;

    r.skipCurrentElement();
    return new Node(node, name, res, virt);
}

Node* Node::deepClone() const
{ return new Node(*this); }
