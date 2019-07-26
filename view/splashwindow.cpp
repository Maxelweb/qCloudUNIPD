#include "splashwindow.h"
#include "mainwindow.h"


SplashWindow::SplashWindow(QString logoPath, QString stylePath, int secs)
    : QSplashScreen(),
      logo(logoPath),
      stile(stylePath),
      delaySec(secs)
{
    setCursor(Qt::BusyCursor);
    updateMessage("Inizializzazione...");
}

bool SplashWindow::loadStyle(QApplication & app)
{
    if (!stile.open(QFile::ReadOnly))
        return 0;

    QString StyleSheet(stile.readAll());
    app.setStyleSheet(StyleSheet);
    return 1;
}

void SplashWindow::updateMessage(QString mex)
{
    showMessage(mex, Qt::AlignBottom | Qt::AlignHCenter, QColor("#4e5257"));
}


void SplashWindow::execute(QApplication & app, QWidget & view)
{
    setPixmap(logo);
    show();

    QThread::sleep(1);
    app.processEvents(QEventLoop::AllEvents);
    app.processEvents(QEventLoop::AllEvents);

    updateMessage("Caricamento stile...");
    if(!loadStyle(app))
    {
        updateMessage("Caricamento stile fallito!");
        QThread::sleep(3);
    }

    QThread::sleep(1);

    updateMessage("Caricamento...");
    QThread::sleep(delaySec);
    updateMessage("Programma caricato.");

    finish(&view);
    view.show();
}
