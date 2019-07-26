#ifndef QLABELTIME_H
#define QLABELTIME_H

#include <QLabel>
#include <QTime>

// Classe ausiliaria che mostra il tempo corrente a seguito del settaggio del testo

class QLabelTime : public QLabel
{
    public:
        QLabelTime(const QString & = "", QWidget * = nullptr);
        ~QLabelTime() = default;
        void setText(const QString &);
};

#endif // QLABELTIME_H
