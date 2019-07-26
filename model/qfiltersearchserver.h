#ifndef QFILTERSEARCHSERVER_H
#define QFILTERSEARCHSERVER_H

#include <QSortFilterProxyModel>
class QComboBox;

// Classe Filter-Proxy usata per la ricerca dei server

class QFilterSearchServer : public QSortFilterProxyModel
{
    private:
        const QComboBox * searchType;
    protected:
        bool filterAcceptsRow(int, const QModelIndex&) const override;
    public:
        QFilterSearchServer(QObject* = nullptr, const QComboBox* = nullptr);
        bool insertRows(int, int = 1, const QModelIndex& = QModelIndex()) override;
        void removeServerResults();
};

#endif // QFILTERSEARCHSERVER_H
