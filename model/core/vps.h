#ifndef VPS_H
#define VPS_H

#define VPS_BasePrice   3.5
#define VPS_MaxSpeed    2.1
#define VPS_Weight      2
#define VPS_BasePrice_PerSnapshots  2.0
#define VPS_BasePrice_AntiDDoS      10.0

#include "server.h"

// Classe derivata diretta
// La classe VPS identifica tutti i server di tipo Virtual Private Server

class VPS : public Server
{
    friend class StaticTypeBuilder<VPS>;

    private:

        unsigned short Snapshots;
        bool AntiDDoS;
        virtual VPS * createObjectFromData(QXmlStreamReader &) const override;

        static StaticTypeBuilder<VPS> initType;

    public:

        VPS(char ='0', QString ="none", QString ="0.0.0.0",
            unsigned short =1, double =0.5, unsigned short =20,
            unsigned short =0, bool =false);
        VPS(const VPS &) = default;
        ~VPS() = default;

        unsigned short getSnapshots() const;
        bool hasAntiDDoS() const;

        void setSnapshots(unsigned short);
        void setAntiDDoS(bool);

        virtual double calculatePrice() const override;
        virtual double serverSpeed() const override;
        virtual unsigned short resourcesWeight() const override;

        virtual VPS * deepClone() const override;
        virtual QString staticType() const override;

        virtual unsigned short minCores() const override;
        virtual unsigned short maxCores() const override;

        virtual double minRam() const override;
        virtual double maxRam() const override;

        virtual unsigned short minDisk() const override;
        virtual unsigned short maxDisk() const override;

        virtual bool onlyVirtualized() const override;

        virtual void dataSerialize(QXmlStreamWriter&) const override;

        virtual bool operator ==(const Server &) const override;
        virtual bool operator !=(const Server &) const override;

};


#endif // VPS_H
