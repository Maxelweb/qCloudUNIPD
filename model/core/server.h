#ifndef Server_H
#define Server_H

#define Server_BasePrice_PerCPU 2.5
#define Server_BasePrice_PerRAM 2.0
#define Server_BasePrice_PerDISK 1.5

#include "../template/statictypebuilder.hpp"
#include <QString>
#include <map>

class QXmlStreamReader;
class QXmlStreamWriter;

// Classe base virtuale
// Server contiene una versione generica del prodotto da aggiungere

class Server
{
    private:

        char Node;
        QString Label;
        QString IP;
        unsigned short Cores;
        double Ram;
        unsigned short Disk;
        virtual Server * createObjectFromData(QXmlStreamReader&) const = 0;

    protected:

        const double BasePrice;
        const double MaxCPUSpeed;

        static std::map<QString, Server *> * itemsMap;

    public:

        Server(char, QString, QString,
               unsigned short, double, unsigned short,
               double=0.0, double=1.0);
        Server(const Server &) = default;
        virtual ~Server() = default;

        char getNode() const;
        QString getLabel() const;
        QString getIP() const;
        unsigned short getCores() const;
        double getRam() const;
        unsigned short getDisk() const;

        void setNode(char);
        void setLabel(QString);
        void setIP(QString);
        void setCores(unsigned short);
        void setRam(double);
        void setDisk(unsigned short);

        virtual unsigned short minCores() const = 0;
        virtual unsigned short maxCores() const = 0;
        virtual unsigned short multiplierCores() const;

        virtual double minRam() const = 0;
        virtual double maxRam() const = 0;
        virtual double multiplierRam() const;

        virtual unsigned short minDisk() const = 0;
        virtual unsigned short maxDisk() const = 0;
        virtual unsigned short multiplierDisk() const;

        virtual bool onlyVirtualized() const = 0;

        virtual double baseSpecsPrice(double = Server_BasePrice_PerCPU,
                                      double = Server_BasePrice_PerRAM,
                                      double = Server_BasePrice_PerDISK) const;

        virtual double calculatePrice() const = 0;
        virtual double serverSpeed() const = 0;
        virtual unsigned short resourcesWeight() const = 0;

        virtual Server * deepClone() const = 0;

        static Server* dataUnserialize(QXmlStreamReader&); // + createObjectFromData
        virtual void dataSerialize(QXmlStreamWriter&) const = 0;

        virtual QString staticType() const = 0;
        static const std::map<QString, Server *> * staticMap();

        virtual bool operator ==(const Server &) const;
        virtual bool operator !=(const Server &) const;

};

#endif // Server_H
