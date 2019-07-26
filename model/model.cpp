#include "model.h"

Model::Model()
    : ProgramName(qProgramName),
      Credits(qAuthor),
      Saved(false),
      file_server(),
      ServerContainer(),
      NodesContainer()
{ }

Model * Model::deepClone() const
{
    return new Model(*this);
}

void Model::setSaved(bool s)
{
    if(Saved != s) Saved = s;
}

QString Model::getFilepath() const
{
    return file_server;
}

bool Model::saveFileUpdated() const
{
    return Saved;
}

bool Model::newFile(const QString & filename)
{
    if(filename.isEmpty())
        return false;

    Plist<DeepPtr<Server>> vuota;

    XmlIO filer(filename);
    return filer.writeServer(vuota);
}

void Model::loadFile(const QString & filename)
{    
    if(file_server.isEmpty() && filename.isEmpty())
        return;

    XmlIO filer(filename); // Prendo le posizioni dei file di default
    try
    {
        ServerContainer = filer.readServer();
        NodesContainer = filer.readNodes();
        setSaved(true);
        setFilepath(filename); // Cambio il filepath corrente
    }
    catch (int n)
    {
        if(n == 1)
            setSaved(false);
    }
}

void Model::saveFile(const QString & filename)
{
    if(file_server.isEmpty() && filename.isEmpty())
        return;

    XmlIO filer(filename.isEmpty() ? file_server : filename);
    if(filer.writeServer(ServerContainer))
        setSaved(true);
}

bool Model::exportFile(const QString & filename)
{
    if(filename.isEmpty())
        return false;

    XmlIO filer(filename);
    return filer.writeServer(ServerContainer);
}

void Model::setFilepath(const QString & f)
{
    file_server = f;
}

QString Model::getCredits() const
{ return Credits; }

QString Model::getProgramName() const
{ return ProgramName; }

int Model::countNodes() const
{ return NodesContainer.getCounter(); }

int Model::countServer() const
{ return ServerContainer.getCounter(); }

int Model::countWarnings() const
{ return Warnings.size(); }

const std::vector<QString> Model::getWarnings() const
{ return const_cast<std::vector<QString>&>(Warnings); } // Copia costante

Node * Model::getNode(unsigned int i) const
{ return i < static_cast<unsigned int>(countNodes()) ? NodesContainer[i].operator->() : nullptr; }

Server * Model::getServer(unsigned int i) const
{ return i < static_cast<unsigned int>(countServer()) ? ServerContainer[i].operator->() : nullptr; }

unsigned short Model::node_countServer(char n) const
{ return (ServerPerNode.find(n) != ServerPerNode.end()) ? (ServerPerNode.find(n))->second : 0; }

unsigned short Model::node_countUsedResources(char n) const
{ return (UsedResourcesPerNode.find(n) != UsedResourcesPerNode.end()) ? (UsedResourcesPerNode.find(n))->second : 0; }

short Model::node_countRemainingResources(char n) const
{ return (RemainingResourcesPerNode.find(n) != RemainingResourcesPerNode.end()) ? (RemainingResourcesPerNode.find(n))->second : 0; }

const std::vector<char> Model::getNodesIdList() const
{
    std::vector<char> nodi;
    for(auto it = NodesContainer.iBegin(); !it.hasEnded(); ++it)
    {
        nodi.push_back((*it)->getId());
    }
    return const_cast<std::vector<char>&>(nodi);
}

Node * Model::getNodeById(char id) const
{
    auto it = NodesContainer.iBegin();

    while(!it.hasEnded() && (*it)->getId() != id)
        ++it;

    return (*it)->getId() == id ? NodesContainer.Select(it.getPos()).operator->() : nullptr;
}

bool Model::ipChecker(QString ip) const
{
    bool found = false;
    for(auto i = ServerContainer.iBegin(); !i.hasEnded() && !found; ++i)
    {
        if((*i)->getIP() == ip)
            found = true;
    }
    return found;
}

bool Model::labelChecker(QString lab) const
{
    bool found = false;
    for(auto i = ServerContainer.iBegin(); !i.hasEnded() && !found; ++i)
    {
        if((*i)->getLabel() == lab)
            found = true;
    }
    return found;
}

void Model::stats_update()
{
     createMap_serverPerNode();
     createMap_usedResourcesPerNode();
     createMap_remainingResourcesPerNode();
     createVector_warnings();
}

void Model::createMap_serverPerNode()
{
    std::map<char, unsigned short> mappa;

    for(auto i = NodesContainer.iBegin(); !i.hasEnded(); ++i)
    {
        mappa[(*i)->getId()] = 0;
    }

    for(auto it = ServerContainer.iBegin(); !it.hasEnded(); ++it)
    {
        if(mappa.find((*it)->getNode()) != mappa.end())
        {
            mappa[(*it)->getNode()]++;
        }
    }

    ServerPerNode = mappa;
}

void Model::createMap_usedResourcesPerNode()
{
    std::map<char, unsigned short> mappa;

    for(auto i = NodesContainer.iBegin(); !i.hasEnded(); ++i)
    {
        mappa[(*i)->getId()] = 0;
    }

    for(auto it = ServerContainer.iBegin(); !it.hasEnded(); ++it)
    {
        if(mappa.find((*it)->getNode()) != mappa.end())
        {
           mappa[(*it)->getNode()] += 1*((*it)->resourcesWeight());
        }
    }

    UsedResourcesPerNode = mappa;
}

void Model::createMap_remainingResourcesPerNode()
{
    std::map<char, short> mappa;

    for(auto i = NodesContainer.iBegin(); !i.hasEnded(); ++i)
    {
        mappa[(*i)->getId()] = (*i)->getLimits();
    }

    for(auto it = ServerContainer.iBegin(); !it.hasEnded(); ++it)
    {
        if(mappa.find((*it)->getNode()) != mappa.end())
        {
           mappa[(*it)->getNode()] -= 1*((*it)->resourcesWeight());
        }
    }

    RemainingResourcesPerNode = mappa;
}

void Model::createVector_warnings()
{
    std::vector<QString> avvisi;

    for(auto i = ServerPerNode.begin(); i!=ServerPerNode.end(); ++i)
    {
        if(i->second == 0)
        {
            QString m = QString("Il nodo '%1' non viene sfruttato da nessun servizio.").arg(i->first);
            avvisi.push_back(m);
        }
    }

    for(auto it = RemainingResourcesPerNode.begin(); it!=RemainingResourcesPerNode.end(); ++it)
    {
        if(it->second < 0)
        {
            QString m = QString("Il nodo '%1' eccede di risorse limite (+%2)!").arg(it->first).arg(it->second*(-1));
            avvisi.push_back(m);
        }

        if(it->second == 0)
        {
            QString m = QString("Il nodo '%1' utilizza tutte le risorse disponibili.").arg(it->first);
            avvisi.push_back(m);
        }
    }

    Warnings = avvisi;
}

double Model::stats_getTotalNodesCost(double costokwh, bool idle) const
{
    double cost = 0.0;

    for(auto it = NodesContainer.iBegin(); !it.hasEnded(); ++it)
    {
        cost += (*it)->getCosto(costokwh, idle);
    }

    return cost;
}

double Model::stats_getTotalNodesConsume(bool idle) const
{
    double cost = 0.0;

    for(auto it = NodesContainer.iBegin(); !it.hasEnded(); ++it)
    {
        cost += (*it)->getConsumo(idle);
    }

    return cost;
}

double Model::stats_getTotalServerEarn() const
{
    double earn = 0.0;

    for(auto it = ServerContainer.iBegin(); !it.hasEnded(); ++it)
    {
        earn += (*it)->calculatePrice();
    }

    return earn;
}

double Model::stats_getGuadagnoNetto(double costokwh)
{
    return stats_getTotalServerEarn() -
            ((stats_getTotalNodesCost(costokwh, true) + stats_getTotalNodesCost(costokwh, false)) / 2.0);
}

QString Model::stats_getMostUsedService() const
{
    std::map<QString, unsigned short> counter;
    QString key;
    unsigned short val = 0;

    for(auto it = ServerContainer.iBegin(); !it.hasEnded(); ++it)
    {
        if(counter.find((*it)->staticType()) != counter.end())
            counter[(*it)->staticType()]++;
        else
           counter[(*it)->staticType()] = 1;
    }

    for(auto it = counter.begin(); it != counter.end(); ++it)
    {
        if(it->second >= val)
        {
            val = it->second;
            key = it->first;
        }
    }

    return key;
}

char Model::stats_getMostUsedNode() const
{
    char node = '0';
    short val = 0;

    for(auto it = UsedResourcesPerNode.begin(); it != UsedResourcesPerNode.end(); ++it)
    {
        if(it->second >= val)
        {
            val = it->second;
            node = it->first;
        }
    }

    return node;
}

char Model::stats_getLessUsedNode() const
{
    char node = '0';
    short val = 0;

    for(auto it = UsedResourcesPerNode.begin(); it != UsedResourcesPerNode.end(); ++it)
    {
        if(it->second <= val)
        {
            val = it->second;
            node = it->first;
        }
    }

    return node;
}

unsigned int Model::stats_getTotalResources() const
{
    unsigned int tot = 0;

    for(auto it = NodesContainer.iBegin(); !it.hasEnded(); ++it)
    {
        tot += (*it)->getLimits();
    }

    return tot;
}

unsigned int Model::stats_getTotalUsedResources() const
{
    unsigned int tot = 0;

    for(auto it = UsedResourcesPerNode.begin(); it != UsedResourcesPerNode.end(); ++it)
    {
        tot += it->second;
    }

    return tot;
}

double Model::stats_getPercentageResourcesUsed() const
{
    if(countNodes() == 0)
        return 0.0;
    return (static_cast<double>(stats_getTotalUsedResources()) / static_cast<double>(stats_getTotalResources())) * 100.0;
}

bool Model::addServer(Server * s)
{
    if(s == nullptr)
        return false;
    ServerContainer.pushFront(DeepPtr<Server>(s));
    setSaved(false);
    return true;
}

// return static_cast<bool>(0);

bool Model::removeServer(unsigned int i)
{
    if(!ServerContainer.RemoveFromPos(i))
        return false;

    setSaved(false);
    return true;
}



