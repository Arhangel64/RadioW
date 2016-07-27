#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QListView>
#include <QtCore/QAbstractListModel>
#include <QtWidgets/QPlainTextEdit>

//#include "applistitemdelegate.h"
#include "detailedview.h"

class MainView : public QWidget
{
    Q_OBJECT
    
public:
    MainView(QAbstractListModel* model, QWidget* parent = 0);
    
public:
    QSplitter* splitter;
    QListView* list;
    DetailedView* details;
    
private:
    bool detailed;
    
public:
    void showDetails();
    void hideDetails();
    
};

#endif // MAINVIEW_H
