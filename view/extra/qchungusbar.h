#ifndef QCHUNGUSBAR_H
#define QCHUNGUSBAR_H


#include <QProgressBar>

// Classe ausiliaria per le barre di completamento

class QChungusBar : public QProgressBar
{
    public:
        QChungusBar(unsigned short=0, unsigned short=0, unsigned short=0, QWidget * = nullptr);
        void applyCustomStyle(bool =false);
};

#endif // QCHUNGUSBAR_H
