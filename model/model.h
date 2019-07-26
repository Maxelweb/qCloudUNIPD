#ifndef MODEL_H
#define MODEL_H

#define qProgramName "qCloud Manager"
#define qAuthor "Mariano Sciacco"

#include "template/plist.hpp"
#include "template/deepptr.hpp"
#include "core/server.h"
#include "core/node.h"
#include "xmlio.h"

#include <QString>
#include <map>
#include <vector>

// Classe Model
// Racchiude tutte le funzionalità del programma ed è indipendente dalla view (MainWindow)

class Model
{
    private:
        const QString ProgramName;
        const QString Credits;
        bool Saved;
        QString file_server;
        Plist<DeepPtr<Server>> ServerContainer;
        Plist<DeepPtr<Node>> NodesContainer;

        std::map<char, unsigned short> ServerPerNode;
        std::map<char, unsigned short> UsedResourcesPerNode;
        std::map<char, short> RemainingResourcesPerNode;

        std::vector<QString> Warnings;

        void createMap_serverPerNode();
        void createMap_usedResourcesPerNode();
        void createMap_remainingResourcesPerNode();
        void createVector_warnings();

    public:

        Model();
        ~Model() = default;
        Model * deepClone() const;

        // Gestione file (xmlio)

        bool saveFileUpdated() const;
        bool newFile(const QString & ="");
        void loadFile(const QString & ="");
        void saveFile(const QString & ="");
        bool exportFile(const QString &);
        void setFilepath(const QString &);
        void setSaved(bool);
        QString getFilepath() const;

        // Gestione modello

        QString getCredits() const;
        QString getProgramName() const;

        int countNodes() const; // non 'unsigned' per supportare index.row() nell'adapter
        int countServer() const;

        Node* getNode(unsigned int) const;
        Server* getServer(unsigned int) const;

        unsigned short node_countServer(char) const;
        unsigned short node_countUsedResources(char) const;
        short node_countRemainingResources(char) const;        

        const std::vector<char> getNodesIdList() const;
        Node* getNodeById(char) const;

        bool ipChecker(QString) const;
        bool labelChecker(QString) const;

        // Avvisi

        int countWarnings() const;
        const std::vector<QString> getWarnings() const;

        // Statistiche

        void stats_update();

        double stats_getTotalNodesCost(double, bool) const; // costokwh, idle
        double stats_getTotalNodesConsume(bool) const; // idle
        double stats_getTotalServerEarn() const;
        double stats_getGuadagnoNetto(double); // costokwh

        QString stats_getMostUsedService() const;
        char stats_getMostUsedNode() const;
        char stats_getLessUsedNode() const;

        unsigned int stats_getTotalUsedResources() const;
        unsigned int stats_getTotalResources() const;
        double stats_getPercentageResourcesUsed() const;

        // Operazioni

        bool addServer(Server *);
        bool removeServer(unsigned int);
};

#endif // MODEL_H
