#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QModelIndex>
#include "../model/template/deepptr.hpp"
#include "../model/model.h"

class QTableView;
class QGroupBox;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QLabel;
class QLabelTime;
class QTableServerAdapter;
class QTableNodesAdapter;
class QFilterSearchServer;
class QComboBox;
class QLineEdit;
class QProgressBar;

#define DefaultKWHCost 0.089

class MainWindow : public QWidget
{
    Q_OBJECT
    private:
        DeepPtr<Model> model; // Il modello viene distrutto alla chiusura del programma in quanto l'istanza rimane invariata
        QTableView * nodesTable;
        QTableView * serverTable;
        QHBoxLayout * layoutGeneralPage;
        QTabWidget * tabs;
        QLabel * sectionInfoTitle;
        QVBoxLayout * viewInfo;
        QHBoxLayout * layoutBody;
        QGridLayout *layoutTopRightNotification;
        QLabelTime * textFooterRight;
        QTableServerAdapter * serverAdapter;
        QFilterSearchServer * serverProxy;
        QTableNodesAdapter * nodesAdapter;
        QLineEdit * fSearch;
        QComboBox * fSelect;
        QLabel * notWarnings;
        QLabel * warnImage;
        QLabel * notServers;
        QLabel * notNodes;

        // Metodi ausiliari

        void populate_nodesComboBox(QComboBox *);
        void create_notificationBar();
        unsigned int getServerSelectionRow() const;
        QModelIndex getServerSelection() const;
        void setFileNotification(const QString &);

    protected:
        virtual void closeEvent (QCloseEvent *event) override;

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        static void cleanLayout(QLayout *);

    private slots:

        // Gestione view

        void updateSectionTitle();
        void updateNotification();
        void updateGeneralPage();
        void showInfoBox() const;
        void showFileInfoBox() const;
        void showServerDetails();
        void showServerEditDetails();
        bool showServerDeleteConfirm();
        bool showServerDeleteAllConfirm();
        void showServerAddDetails();
        void showNodeDetails();
        void detailAutoCleaner();
        void UpdateSearchFilter();

        // Gestione file (view)

        void pokeToCreateNewData();
        void pokeModelToLoadData(const QString &);
        void pokeModelToSaveData();
        void pokeModelToSaveAsData();
        void pokeModelToExportData();
        void openDialogToLoadData();
        QString openDialogToNewSaveFile();
        void checkUnsavedData();
        bool checkFilepath();

    public slots:

        // Gestione aggiornamento modello-view

        void UpdateServer();
        void DeleteServer();
        void AddServer();
        void DeleteServerResults();

};

#endif // MAINWINDOW_H
