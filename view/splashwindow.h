#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#define qDefaultStylePath ":res/styles/stylesheet.css"
#define qDefaultSplashPath ":res/styles/images/splash.png"

#include <QString>
#include <QSplashScreen>
#include <QThread>
#include <QFile>
#include <QApplication>

// Classe usata per caricare lo stile e la view iniziale con MainWindow

class MainWindow;

class SplashWindow : public QSplashScreen
{
    private:
        QPixmap logo;
        QFile stile;
        int delaySec;
        bool loadStyle(QApplication &);
        void updateMessage(QString);

    public:
        SplashWindow(QString =qDefaultSplashPath, QString =qDefaultStylePath, int =3);
        ~SplashWindow() = default;
        void execute(QApplication &, QWidget &);
};

#endif // SPLASHWINDOW_H
