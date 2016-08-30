#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include "views/mainview.h"
#include "views/newappdialogue.h"
#include "models/applistmodel.h"
#include "models/appmodel.h"
#include "models/service.h"

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
    
    
signals:
    void addService(const QMap<QString, QString>&);
    
public slots:
    void robouteMessage(const QString& msg);
    void newService(const Service& srv);
    
private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void unselectAll();
    
    void newApplication();
    void newAppAccepted();
    void newAppRejected();
};

#endif // MAINWINDOW_H
