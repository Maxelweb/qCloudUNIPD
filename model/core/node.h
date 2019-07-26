#ifndef NODE_H
#define NODE_H

#include<QString>

class QXmlStreamReader;
class QXmlStreamWriter;

// Classe base non derivata e senza derivazioni
// Questa classe identifica i nodi hardware aziendali in cui sono inseriti i server della gerarchia

class Node
{
    private:
        char Id;
        QString Name;
        unsigned short ResourcesLimit;
        bool Virtualized;

    public:
        Node(char, QString, unsigned short, bool);
        Node(const Node &) = default;
        ~Node() = default;
        Node * deepClone() const;

        char getId() const;
        QString getName() const;
        unsigned short getLimits() const;
        bool isVirtualized() const;

        double getConsumo(bool) const;
        double getCosto(double, bool=false) const;

        static Node* dataUnserialize(QXmlStreamReader &);
};

#endif // NODE_H
