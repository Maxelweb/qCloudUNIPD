#include "qtablenodesadapter.h"
#include "model.h"
#include <QFont>
#include <QColor>
#include <QBrush>
#include <QVariant>

// Adattatore di riferimento al modello per:
//  TABELLA NODI

QTableNodesAdapter::QTableNodesAdapter(Model * modello, QObject* parent) :
    QAbstractTableModel(parent),
    model(modello)
{ }

int QTableNodesAdapter::rowCount(const QModelIndex &) const {
    return model->countNodes();
}

int QTableNodesAdapter::columnCount(const QModelIndex &) const {
    return 4;
}

QVariant QTableNodesAdapter::headerData(int section, Qt::Orientation orientation, int role) const
{

    if(role == Qt::FontRole)
        return QFont("Sans Serif", 13, QFont::Bold);

    if(role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return QString("ID");
            case 1:
                return QString("Etichetta");
            case 2:
                return QString("Risorse usate");
            case 3:
                return QString("Ambiente");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

QVariant QTableNodesAdapter::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= model->countNodes() || index.row()<0)
        return QVariant();

    unsigned short usedRes = model->node_countUsedResources(model->getNode(static_cast<unsigned int>(index.row()))->getId());
    unsigned short totalRes = model->getNode(static_cast<unsigned int>(index.row()))->getLimits();

    switch(role)
    {
        case Qt::BackgroundRole:
            if(usedRes >= totalRes) // Nodo pieno o con risorse in eccesso
                return QVariant(QBrush(QColor("#e8cdc9")));
            else if(usedRes == 0) // Nessuna risorsa assegnata
                return QVariant(QBrush(QColor("#FFF")));
            else // Nodo con risorse assegnate correttamente
                return QVariant(QBrush(QColor("#e0e8d5")));
        case Qt::TextAlignmentRole:
            return QVariant(Qt::AlignVCenter | Qt::AlignHCenter);
        case Qt::SizeHintRole:
            return QSize(500, 0);
        case Qt::EditRole:
            return QVariant();
        case Qt::DisplayRole:
        {
            switch(index.column())
            {

            case 0:
                return QString(model->getNode(static_cast<unsigned int>(index.row()))->getId());
            case 1:
                return model->getNode(static_cast<unsigned int>(index.row()))->getName();
            case 2:
                return QString("%1 / %2")
                        .arg(usedRes)
                        .arg(totalRes);
            case 3:
                return model->getNode(static_cast<unsigned int>(index.row()))->isVirtualized() ? "Virtualizzato" : "Dedicato";
            default:
                return QVariant();
            }
        }

        case Qt::FontRole:
        {
            QFont font;
            if (!model->getNode(static_cast<unsigned int>(index.row()))->isVirtualized())
                font.setItalic(true);
            return font;
        }
    }

    return QVariant();
}

void QTableNodesAdapter::dataRefresh()
{
    beginResetModel();
    endResetModel();
}
