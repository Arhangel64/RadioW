#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QSettings>

#include "views/mainview.h"
#include "views/newappdialogue.h"
#include "models/applistmodel.h"
#include "models/appmodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow();
    
private:
    AppListModel* apps;
    MainView* widget;
    NewAppDialogue* newApp;
    
private:
    void createActions();
    
    void subscribeDetailsById(quint64 id);
    void unsubscribeDetailsById(quint64 id);
    void restoreSettings();
    
signals:
    void addService(const QMap<QString, QString>&);
    void connectService(uint64_t);
    void disconnectService(uint64_t);
    void launchService(uint64_t);
    void stopService(uint64_t);
    void removeService(uint64_t id);
    
public slots:
    void saveSettings();
    void robouteMessage(const QString& msg);
    void newService(uint64_t id, const QString& name);
    void serviceAttrChange(uint64_t id, const QString& key, const QString& value);
    void serviceConnected(uint64_t id);
    void serviceConnecting(uint64_t id);
    void serviceDisconnected(uint64_t id);
    void serviceDisconnecting(uint64_t id);
    void serviceConnectionFailed(uint64_t id);
    void serviceLaunched(uint64_t id);
    void serviceLaunching(uint64_t id);
    void serviceStopped(uint64_t id);
    void serviceStopping(uint64_t id);
    void serviceLog(uint64_t id, const QString& log);
    void serviceRemoved(uint64_t id);
    
private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void unselectAll();
    
    void newApplication();
    void newAppAccepted();
    void newAppRejected();
};

#endif // MAINWINDOW_H
