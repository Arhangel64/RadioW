#include <QMenuBar>

#include "mainwindow.h"
#include <iostream>


MainWindow::MainWindow():
    QMainWindow(),
    apps(new AppListModel(this)),
    widget(new MainView(apps, this)),
    newApp(0),
    commandForm(0),
    rightBar(new QToolBar(this))
{
    createActions();
    createToolbar();
    setCentralWidget(widget);
    
    apps->push_back(0, "Roboute");
    apps->setLaunched(0, true);
    apps->setConnected(0, true);
    
    QItemSelectionModel* as = widget->list->selectionModel();
    
    connect(
        as, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
        this, SLOT(selectionChanged(const QItemSelection&, const QItemSelection&))
    );
    connect(widget->details, SIGNAL(connect(uint64_t)), this, SIGNAL(connectService(uint64_t)));
    connect(widget->details, SIGNAL(disconnect(uint64_t)), this, SIGNAL(disconnectService(uint64_t)));
    connect(widget->details, SIGNAL(launch(uint64_t)), this, SIGNAL(launchService(uint64_t)));
    connect(widget->details, SIGNAL(stop(uint64_t)), this, SIGNAL(stopService(uint64_t)));
    connect(widget->details, SIGNAL(remove(uint64_t)), this, SIGNAL(removeService(uint64_t)));
    connect(widget->details, SIGNAL(launchCommand(uint64_t, const QString&)), this, SLOT(onLaunchedCommand(uint64_t, const QString&)));
    
    restoreSettings();
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
        rightBar->hide();
    } else if (deselectedIndexes.size() == 0 && selectedIndexes.size() == 1) {
        widget->showDetails();
        rightBar->show();
    }
}

void MainWindow::subscribeDetailsById(quint64 id)
{
    widget->details->setModel(apps->getApp(id));
}

void MainWindow::unsubscribeDetailsById(quint64 id)
{
    widget->details->clearModel();
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

void MainWindow::createToolbar()
{
    addToolBar(Qt::RightToolBarArea, rightBar);
    rightBar->setMovable(false);
    rightBar->setObjectName("rightBar");
    rightBar->hide();
    
    QAction* attrs = rightBar->addAction(QIcon::fromTheme("dialog-object-properties"), tr("Attributes"));
    QAction* commands = rightBar->addAction(QIcon::fromTheme("dialog-scripts"), tr("Commands"));
    
    attrs->setCheckable(true);
    commands->setCheckable(true);
    
    QActionGroup* ag = new QActionGroup(rightBar);
    ag->setExclusive(true);
    ag->addAction(attrs);
    ag->addAction(commands);
    
    connect(attrs, SIGNAL(toggled(bool)), SLOT(attrsToggled(bool)));
    connect(commands, SIGNAL(toggled(bool)), SLOT(commandsToggled(bool)));
    
}


void MainWindow::newApplication()
{
    newApp = new NewAppDialogue(this);
    connect(newApp, SIGNAL(accepted()), SLOT(newAppAccepted()));
    connect(newApp, SIGNAL(rejected()), SLOT(newAppRejected()));
    newApp->setModal(true);
    newApp->setWindowTitle(tr("New application"));
    newApp->show();
}

void MainWindow::newAppAccepted()
{
    emit addService(newApp->getData());
    delete newApp;
    newApp = 0;
}

void MainWindow::newAppRejected()
{
    delete newApp;
    newApp = 0;
}

void MainWindow::newService(uint64_t id, const QString& name)
{
    apps->push_back(id, name);
    apps->setConnectable(id, true);
}

void MainWindow::serviceConnecting(uint64_t id)
{
    apps->setConnectable(id, false);
    apps->setConnected(id, false);
}

void MainWindow::serviceConnected(uint64_t id)
{
    apps->setConnectable(id, true);
    apps->setConnected(id, true);
}

void MainWindow::serviceDisconnecting(uint64_t id)
{
    apps->setConnectable(id, false);
    apps->setConnected(id, true);
}

void MainWindow::serviceDisconnected(uint64_t id)
{
    apps->setConnectable(id, true);
    apps->setConnected(id, false);
}

void MainWindow::serviceConnectionFailed(uint64_t id)
{
    apps->setConnected(id, false);
}

void MainWindow::serviceLaunched(uint64_t id)
{
    apps->setLaunched(id, true);
    apps->setLaunchable(id, true);
}

void MainWindow::serviceStopped(uint64_t id)
{
    apps->setLaunched(id, false);
    apps->setLaunchable(id, true);
}

void MainWindow::serviceLaunching(uint64_t id)
{
    apps->setLaunched(id, false);
    apps->setLaunchable(id, false);
}

void MainWindow::serviceStopping(uint64_t id)
{
    apps->setLaunched(id, true);
    apps->setLaunchable(id, false);
}

void MainWindow::serviceLog(uint64_t id, const QString& log)
{
    apps->logMessage(id, log);
}

void MainWindow::serviceRemoved(uint64_t id)
{
    apps->removeElement(id);
}

void MainWindow::restoreSettings()
{
    QSettings settings;
    
    restoreGeometry(settings.value("window/geometry").toByteArray());
    restoreState(settings.value("window/state").toByteArray());
    
    widget->readSettings();
    
    rightBar->hide();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.beginGroup("window");
    
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    
    settings.endGroup();
    
    widget->saveSettings();
}

void MainWindow::serviceAttrChange(uint64_t id, const QString& key, const QString& value)
{
    apps->setAttribute(id, key, value);
}

void MainWindow::attrsToggled(bool checked)
{
    widget->details->showAttrs(checked);
}

void MainWindow::commandsToggled(bool checked)
{
    widget->details->showCommands(checked);
}

void MainWindow::serviceAddCommand(uint64_t id, const QString& key, const QMap<QString, uint64_t>& arguments)
{
    apps->addCommand(id, key, arguments);
}

void MainWindow::serviceRemoveCommand(uint64_t id, const QString& key)
{
    apps->removeCommand(id, key);
}

void MainWindow::serviceClearCommands(uint64_t id)
{
    apps->clearCommands(id);
}

void MainWindow::onLaunchedCommand(uint64_t id, const QString& name)
{
    commandForm = new CommandForm(name, apps->getApp(id)->commands.getCommandArgs(name), this);
    connect(commandForm, SIGNAL(accepted()), SLOT(commandFormAccepted()));
    connect(commandForm, SIGNAL(rejected()), SLOT(commandFormRejected()));
    commandForm->setModal(true);
    commandForm->setWindowTitle(tr("Execute the command"));
    commandForm->show();
}

void MainWindow::commandFormAccepted()
{
    emit launchCommand(widget->details->getModelId(), commandForm->getName(), commandForm->getData());
    delete commandForm;
    commandForm = 0;
}

void MainWindow::commandFormRejected()
{
    delete commandForm;
    commandForm = 0;
}

