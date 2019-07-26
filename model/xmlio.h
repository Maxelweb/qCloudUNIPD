#ifndef XMLIO_H
#define XMLIO_H

#include "template/plist.hpp"
#include "template/deepptr.hpp"
#include "core/server.h"
#include "core/node.h"
#include <QString>

#define DefaultNodesFile ":res/data-nodes.xml"

// Classe usata per la gestione dei file XML

class XmlIO
{
    private:
        QString ServerFile;
        QString NodesFile;
    public:
        XmlIO(const QString &, const QString & =DefaultNodesFile);
        Plist<DeepPtr<Node>> readNodes() const;
        Plist<DeepPtr<Server>> readServer() const;
        bool writeServer(const Plist<DeepPtr<Server>>&) const;
};

#endif // XMLIO_H
