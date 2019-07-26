
#ifndef Hosting_H
#define Hosting_H

#define Hosting_BasePrice   7.5
#define Hosting_MaxSpeed    1.2
#define Hosting_Weight      1
#define Hosting_BasePrice_SSHAccess 6.0
#define Hosting_BasePrice_Turbo     5.0

#include "server.h"

// Classe derivata diretta
// La classe Hosting identifica un server a bassa potenza per uso esclusivo web

class Hosting : public Server
{
    friend class StaticTypeBuilder<Hosting>;

    private:

        bool SSHAccess;
        bool TurboBoost;
        virtual Hosting * createObjectFromData(QXmlStreamReader &) const override;

        static StaticTypeBuilder<Hosting> initType;

    public:

        Hosting(char ='0', QString ="none", QString ="0.0.0.0",
                unsigned short =1, double =0.5, unsigned short =20,
                bool =false, bool =false);
        Hosting(const Hosting &) = default;
        ~Hosting() = default;

        bool hasSSHAccess() const;
        bool hasTurboBoost() const;

        void setSSHAccess(bool);
        void setTurboBoost(bool);

        virtual double calculatePrice() const override;
        virtual double serverSpeed() const override;
        virtual unsigned short resourcesWeight() const override;

        virtual Hosting * deepClone() const override;
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

#endif // Hosting_H
