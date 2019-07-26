#ifndef QTableNodesAdapter_H
#define QTableNodesAdapter_H

#include <QAbstractTableModel>
class Model;

// Classe adattatore del modello per i nodi

class QTableNodesAdapter: public QAbstractTableModel
{
    private:
        Model * model;
    public:
        QTableNodesAdapter(Model *, QObject* =nullptr);
        int rowCount(const QModelIndex& = QModelIndex()) const override;
        int columnCount(const QModelIndex& = QModelIndex()) const override;
        QVariant headerData(int, Qt::Orientation, int) const override;
        QVariant data(const QModelIndex&, int = Qt::DisplayRole) const override;
        void dataRefresh();
};

#endif // QTableNodesAdapter_H
