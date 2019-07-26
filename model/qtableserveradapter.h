#ifndef QTABLESERVERADAPTER_H
#define QTABLESERVERADAPTER_H

#include <QAbstractTableModel>
class Model;

// Classe adattatore del modello per i server

class QTableServerAdapter : public QAbstractTableModel
{
    private:
        Model * model;
    public:
        QTableServerAdapter(Model *, QObject* =nullptr);
        int rowCount(const QModelIndex& = QModelIndex()) const override;
        int columnCount(const QModelIndex& = QModelIndex()) const override;
        QVariant headerData(int, Qt::Orientation, int) const override;
        QVariant data(const QModelIndex&, int = Qt::DisplayRole) const override;
        bool insertRows(int, int = 1, const QModelIndex& = QModelIndex()) override;
        bool removeRows(int, int = 1, const QModelIndex& = QModelIndex()) override;
        bool searchMatchRegex(unsigned int, const QRegExp&, const QString&) const;
    public slots:
        void currentRowToUpdate(const QModelIndex &);
        void dataRefresh();
};

#endif // QTABLESERVERADAPTER_H
