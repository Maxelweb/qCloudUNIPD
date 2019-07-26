#include "view/mainwindow.h"
#include "view/splashwindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MainWindow view;
    SplashWindow splash;
    splash.execute(application, view);
    return application.exec();
}

