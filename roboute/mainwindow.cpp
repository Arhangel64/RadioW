#include <QMenuBar>

#include "mainwindow.h"
#include <iostream>


MainWindow::MainWindow():
    QMainWindow(),
    apps(new AppListModel(this)),
    widget(new MainView(apps, this)),
    newApp(0),
    detalizedId(0)
{
    createActions();
    setCentralWidget(widget);
    
    apps->push_back(0, "Roboute");
    apps->setLaunched(0, true);
    apps->setConnected(0, true);
    
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
    widget->details->setConnectable(app->getConnectable());
    widget->details->setConnected(app->getConnected());
    widget->details->setLaunchable(app->getLaunchable());
    widget->details->setLaunched(app->getLaunched());
    delete history;
    detalizedId = id;
    connect(app, SIGNAL(newLogMessage(const QString&)), widget->details, SLOT(appendMessage(const QString&)));
    connect(app, SIGNAL(changedConnectable(bool)), widget->details, SLOT(setConnectable(bool)));
    connect(app, SIGNAL(changedConnected(bool)), widget->details, SLOT(setConnected(bool)));
    connect(app, SIGNAL(changedLaunchable(bool)), widget->details, SLOT(setLaunchable(bool)));
    connect(app, SIGNAL(changedLaunched(bool)), widget->details, SLOT(setLaunched(bool)));
}

void MainWindow::unsubscribeDetailsById(quint64 id)
{
    const AppModel* app = apps->getApp(id);
    widget->details->clear();
    disconnect(app, SIGNAL(newLogMessage(const QString&)), widget->details, SLOT(appendMessage(const QString&)));
    disconnect(app, SIGNAL(changedConnectable(bool)), widget->details, SLOT(setConnectable(bool)));
    disconnect(app, SIGNAL(changedConnected(bool)), widget->details, SLOT(setConnected(bool)));
    disconnect(app, SIGNAL(changedLaunchable(bool)), widget->details, SLOT(setLaunchable(bool)));
    disconnect(app, SIGNAL(changedLaunched(bool)), widget->details, SLOT(setLaunched(bool)));
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

void MainWindow::onDetailsConnect()
{
    emit connectService(detalizedId);
}

void MainWindow::onDetailsDisconnect()
{
    emit disconnectService(detalizedId);
}

void MainWindow::onDetailsLaunch()
{
    emit launchService(detalizedId);
}

void MainWindow::onDetailsStop()
{
    emit stopService(detalizedId);
}

void MainWindow::serviceConnected(uint64_t id)
{
    apps->setConnected(id, true);
}

void MainWindow::serviceDisconnected(uint64_t id)
{
    apps->setConnected(id, false);
}

void MainWindow::serviceConnectionFailed(uint64_t id)
{
    apps->setConnected(id, false);
}

void MainWindow::serviceLaunched(uint64_t id)
{
    apps->setLaunched(id, true);
}

void MainWindow::serviceStopped(uint64_t id)
{
    apps->setLaunched(id, false);
}

void MainWindow::serviceLaunchingFailed(uint64_t id)
{
    apps->setLaunched(id, false);
}

void MainWindow::serviceStoppingFailed(uint64_t id)
{
    apps->setLaunched(id, true);
}
