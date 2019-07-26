#include "qlabeltime.h"

QLabelTime::QLabelTime(const QString & str, QWidget * parent)
    : QLabel(parent)
{
    setTextFormat(Qt::RichText);
    setText(str);
}

void QLabelTime::setText(const QString & s)
{
    QTime t = QTime(QTime::currentTime());
    QLabel::setText(QString("%1 <small>(%2)</small>").arg(s).arg(t.toString("hh:mm:ss")));
}

