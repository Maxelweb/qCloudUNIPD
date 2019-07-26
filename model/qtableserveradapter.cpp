#include "qtableserveradapter.h"
#include "model.h"
#include <QFont>
#include <QColor>
#include <QBrush>
#include <QVariant>
#include <QPushButton>
#include <QRegExp>
#include <QDebug>

// Adattatore di riferimento al modello per:
//  TABELLA SERVER

QTableServerAdapter::QTableServerAdapter(Model * modello, QObject* parent) :
    QAbstractTableModel(parent),
    model(modello)
{ }

int QTableServerAdapter::rowCount(const QModelIndex &) const {
    return model->countServer();
}

int QTableServerAdapter::columnCount(const QModelIndex &) const {
    return 7;
}

QVariant QTableServerAdapter::headerData(int section, Qt::Orientation orientation, int role) const
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
                return QString("Tipologia");
            case 1:
                return QString("Etichetta");
            case 2:
                return QString("IP");
            case 3:
                return QString("Nodo");
            case 4:
                return QString("Costo");
            case 5:
                return QString("vCore");
            case 6:
                return QString("RAM");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

QVariant QTableServerAdapter::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= model->countServer() || index.row()<0)
        return QVariant();

    switch(role)
    {
        case Qt::BackgroundRole:
            return QVariant(QBrush(QColor( (index.row() % 2 == 0) ? "#EEE" : "#FFF")));
        case Qt::TextAlignmentRole:
            return QVariant ( Qt::AlignVCenter | Qt::AlignHCenter );
        case Qt::SizeHintRole:
            return QSize(500, 0 );
        case Qt::EditRole:
            return QVariant();
        case Qt::DisplayRole:
        {
            switch(index.column())
            {

            case 0:
                return QString(model->getServer(static_cast<unsigned int>(index.row()))->staticType());
            case 1:
                return model->getServer(static_cast<unsigned int>(index.row()))->getLabel();
            case 2:
                return model->getServer(static_cast<unsigned int>(index.row()))->getIP();
            case 3:
                return QString(model->getServer(static_cast<unsigned int>(index.row()))->getNode());
            case 4:
                return QString("%1 €").arg(QString::number(model->getServer(static_cast<unsigned int>(index.row()))->calculatePrice(), 'f', 2));
            case 5:
                return QString::number(model->getServer(static_cast<unsigned int>(index.row()))->getCores());
            case 6:
                return QString("%1 GB").arg(QString::number(model->getServer(static_cast<unsigned int>(index.row()))->getRam(), 'f', 1));
            default:
                return QVariant();
            }
        }

        case Qt::FontRole:
        {
            QFont font;
            if (model->getServer(static_cast<unsigned int>(index.row()))->staticType() == "Dedicated")
                font.setItalic(true);
            return font;
        }
    }

    return QVariant();
}

void QTableServerAdapter::currentRowToUpdate(const QModelIndex & index)
{
    if (index.isValid()
            && index.row() < rowCount()
            && index.column() < columnCount()
        )
        emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), columnCount()-1));
}

void QTableServerAdapter::dataRefresh()
{
    beginResetModel();
    endResetModel();
}

bool QTableServerAdapter::insertRows(int begin, int count, const QModelIndex& parent) {
    beginInsertRows(parent, begin, begin + count - 1);
    // Aggiunta di nuove righe
    endInsertRows();
    return true;
}

bool QTableServerAdapter::removeRows(int begin, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, begin, begin + count - 1);
    model->removeServer(begin);
    endRemoveRows();
    return true;
}

bool QTableServerAdapter::searchMatchRegex(unsigned int i, const QRegExp & r, const QString & s) const
{
    if(s == "Tutti i nodi")
        return model->getServer(i)->getLabel().contains(r) ||
                model->getServer(i)->getIP().contains(r)  ||
                model->getServer(i)->staticType().contains(r);
    else if(s == QString(model->getServer(i)->getNode()))
        return model->getServer(i)->getLabel().contains(r) ||
                model->getServer(i)->getIP().contains(r) ||
                model->getServer(i)->staticType().contains(r);
    else
        return false;
}
