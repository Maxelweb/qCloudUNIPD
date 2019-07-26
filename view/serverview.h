#ifndef SERVERVIEW_H
#define SERVERVIEW_H

#include <QObject>
#include <QWidget>
#include <QModelIndex>
class Server;
class Model;
class QComboBox;
class QLineEdit;
class QLabel;
class QCheckBox;
class QSpinBox;
class QRegExpValidator;
class QHBoxLayout;

class ServerView : public QWidget
{
    Q_OBJECT

    private:
        Model * model;
        Server * server;
        QLabel * error;

        QLineEdit * etichetta;
        QComboBox * tipo;
        QLineEdit * ip;
        QComboBox * nodes;
        QComboBox * cores;
        QComboBox * ram;
        QComboBox * disk;

        QSpinBox * vpsSNAP;
        QCheckBox * vpsDDOS;
        QSpinBox * dedIP;
        QCheckBox * dedBAK;
        QCheckBox * hostSSH;
        QCheckBox * hostBOOST;

        QHBoxLayout * dynamicLayout;

        QRegExpValidator * create_ipValidator() const;
        void update_addServerTemplate(QString);
        void update_currentServer();

    public:
        explicit ServerView(Model *, Server *, QWidget *parent = nullptr);

    signals:
           void pokeUpdateServer();
           void pokeAddServer();

    public slots:
        QLayout * readServerDetail();
        QLayout * editServerDetail();
        QLayout * addServerDetail();
        bool editServerDetail_verify();
        bool addServerDetail_verify();
        bool serverInput_verify();
        void currentTypeChanged();

};

#endif // SERVERVIEW_H
