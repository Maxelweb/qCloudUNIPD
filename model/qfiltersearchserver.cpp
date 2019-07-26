
#include "qfiltersearchserver.h"
#include <QComboBox>
#include "qtableserveradapter.h"

QFilterSearchServer::QFilterSearchServer(QObject* parent, const QComboBox * s)
    : QSortFilterProxyModel (parent), searchType(s)
{ }

bool QFilterSearchServer::insertRows(int begin, int count, const QModelIndex& parent)
{
    bool result = sourceModel()->insertRows(begin, count, parent);
    invalidateFilter();
    return result;
}

bool QFilterSearchServer::filterAcceptsRow(int row_source, const QModelIndex&) const
{
    return static_cast<QTableServerAdapter*>(sourceModel())->searchMatchRegex(static_cast<unsigned int>(row_source),
                                                                        filterRegExp(),
                                                                        searchType->currentText());
}

void QFilterSearchServer::removeServerResults()
{
    while(rowCount())
        removeRows(0,1); // Rimuovo ciascun nodo visualizzato
}
