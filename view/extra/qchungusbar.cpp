#include "qchungusbar.h"
#include <QVariant>

QChungusBar::QChungusBar(unsigned short min, unsigned short max, unsigned short current, QWidget *parent)
    : QProgressBar(parent)
{
    setMaximum(static_cast<int>(max));
    setMinimum(static_cast<int>(min));
    setValue(static_cast<int>(current));
}

void QChungusBar::applyCustomStyle(bool tiny)
{
    if((minimum() == 0 && maximum() == 0) ||
       (maximum() == 0 && value() == 0))
    {
        setProperty("class", tiny ? "progressBarra tiny" : "ProgressBarra");
        return;
    }

    // La percentuale viene settata negativamente quando va oltre il massimo

    double perc = static_cast<double>((value()<0 ? value()*(-1)+maximum() : value()) - minimum()) /
            static_cast<double>(maximum() - minimum()) * 100.0;

    if(perc > 100.0)
    {
        perc = 100.0;
        setValue(maximum());
    }

    QString classes = "progressBarra";

    if(tiny)
        classes += " tiny";

    if(perc > 90.0)
        classes += " danger";
    else if(perc > 60.0)
        classes += " warning";

    setProperty("class", QVariant(classes));
}
