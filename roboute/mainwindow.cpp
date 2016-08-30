#include <QMenuBar>

#include "mainwindow.h"
#include <iostream>


MainWindow::MainWindow():
    QMainWindow(),
    apps(new AppListModel(this)),
    widget(new MainView(apps, this)),
    newApp(0)
{
    createActions();
    setCentralWidget(widget);
    
    apps->push_back(0, "Roboute");
    QItemSelectionModel* as = widget->list->selectionModel();
    
    connect(
        as, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
        this, SLOT(selectionChanged(const QItemSelection&, const QItemSelection&))
    );
}

void MainWindow::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QModelIndexList deselectedIndexes = deselected.indexes();
    QModelIndexList::const_iterator dItr = deselectedIndexes.begin();
    QModelIndexList::const_iterator dEnd = deselectedIndexes.end();
    for (; dItr != dEnd; ++dItr) {
        unsubscribeDetailsById(apps->data(*dItr, Qt::UserRole).toInt());
    }
    
    QModelIndexList selectedIndexes = selected.indexes();
    QModelIndexList::const_iterator sItr = selectedIndexes.begin();
    QModelIndexList::const_iterator sEnd = selectedIndexes.end();
    for (; sItr != sEnd; ++sItr) {
        subscribeDetailsById(apps->data(*sItr, Qt::UserRole).toInt());
    }
    
    if (deselectedIndexes.size() == 1 && selectedIndexes.size() == 0) {
        widget->hideDetails();
    } else if (deselectedIndexes.size() == 0 && selectedIndexes.size() == 1) {
        widget->showDetails();
    }
}

void MainWindow::subscribeDetailsById(quint64 id)
{
    const AppModel* app = apps->getApp(id);
    QString* history = app->getHistory();
    widget->details->appendMessage(*history);
    delete history;
    connect(app, SIGNAL(newLogMessage(const QString&)), widget->details, SLOT(appendMessage(const QString&)));
}

void MainWindow::unsubscribeDetailsById(quint64 id)
{
    const AppModel* app = apps->getApp(id);
    widget->details->clear();
    disconnect(app, SIGNAL(newLogMessage(const QString&)), widget->details, SLOT(appendMessage(const QString&)));
}

void MainWindow::robouteMessage(const QString& msg)
{
    apps->logMessage(0, msg);
}

void MainWindow::unselectAll()
{
    widget->list->selectionModel()->clearSelection();
}

void MainWindow::createActions()
{
    QMenu *actionsMenu = menuBar()->addMenu(tr("Actions"));
    
    const QIcon newIcon = QIcon::fromTheme("document-new");
    QAction *newAct = new QAction(newIcon, tr("New application"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Add new application"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newApplication);
    actionsMenu->addAction(newAct);
}

void MainWindow::newApplication()
{
    newApp = new NewAppDialogue(this);
    connect(newApp, SIGNAL(accepted()), SLOT(newAppAccepted()));
    connect(newApp, SIGNAL(rejected()), SLOT(newAppRejected()));
    newApp->setModal(true);
    newApp->show();
}

void MainWindow::newAppAccepted()
{
    std::cout << "accepted" << std::endl;
    emit addService(newApp->getData());
    delete newApp;
    newApp = 0;
}

void MainWindow::newAppRejected()
{
    std::cout << "rejected" << std::endl;
    delete newApp;
    newApp = 0;
}

void MainWindow::newService(const Service& srv)
{
    apps->push_back(srv.id, srv.name);
}
