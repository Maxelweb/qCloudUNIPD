#ifndef Dedicated_H
#define Dedicated_H

#define Dedicated_BasePrice 35.0
#define Dedicated_MaxSpeed  3.4
#define Dedicated_Weight    4
#define Dedicated_BasePrice_Backup  15.0
#define Dedicated_BasePrice_PerIp   5.0
#define Dedicated_BasePrice_PerCPU  3.5
#define Dedicated_BasePrice_PerRAM  4.5
#define Dedicated_BasePrice_PerDISK 10.0

#include "server.h"

// Classe derivata diretta
// La classe Dedicated identifica server ad alto potenziale e a maggior costo

class Dedicated: public Server
{

    friend class StaticTypeBuilder<Dedicated>;

    private:

        bool Backup;
        unsigned short ExtraIps;
        virtual Dedicated * createObjectFromData(QXmlStreamReader &) const override;

        static StaticTypeBuilder<Dedicated> initType;

    public:

        Dedicated(char ='0', QString ="none", QString ="0.0.0.0",
                  unsigned short =8, double =4.0, unsigned short =240,
                  bool =false, unsigned short =0);
        Dedicated(const Dedicated&) = default;
        ~Dedicated() = default;

        bool hasBackup() const;
        unsigned short getExtraIps() const;

        void setBackup(bool);
        void setExtraIps(unsigned short);

        virtual double baseSpecsPrice(double = Dedicated_BasePrice_PerCPU,
                                      double = Dedicated_BasePrice_PerRAM,
                                      double = Dedicated_BasePrice_PerDISK) const override;

        virtual double calculatePrice() const override;
        virtual double serverSpeed() const override;
        virtual unsigned short resourcesWeight() const override;

        virtual Dedicated * deepClone() const override;
        virtual QString staticType() const override;

        virtual unsigned short minCores() const override;
        virtual unsigned short maxCores() const override;
        virtual unsigned short multiplierCores() const override;

        virtual double minRam() const override;
        virtual double maxRam() const override;
        virtual double multiplierRam() const override;

        virtual unsigned short minDisk() const override;
        virtual unsigned short maxDisk() const override;
        virtual unsigned short multiplierDisk() const override;

        virtual bool onlyVirtualized() const override;

        virtual void dataSerialize(QXmlStreamWriter&) const override;

        virtual bool operator ==(const Server &) const override;
        virtual bool operator !=(const Server &) const override;
};


#endif // Dedicated_H

