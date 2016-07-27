#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include "views/mainview.h"
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
    
private:
    void subscribeDetailsById(quint64 id);
    void unsubscribeDetailsById(quint64 id);
    
    
public slots:
    void robouteMessage(const QString& msg);
    
private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void unselectAll();
};

#endif // MAINWINDOW_H
