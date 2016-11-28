#include "detailedview.h"

#include <QtWidgets/QHeaderView>

DetailedView::DetailedView(QWidget* parent):
    QWidget(parent),
    layout(new QGridLayout(this)),
    topPanel(new QHBoxLayout()),
    logArea(new QPlainTextEdit(this)),
    splitter(new QSplitter(this)),
    props(new QTableView(this)),
    connectBtn(new QPushButton(QIcon::fromTheme("state-ok"), "", this)),
    launchBtn(new QPushButton(QIcon::fromTheme("kt-start"), "", this)),
    removeBtn(new QPushButton(QIcon::fromTheme("remove"), "", this)),
    connected(false),
    launched(false),
    model(0)
{
    setLayout(layout);
    logArea->setReadOnly(true);
    
    layout->addLayout(topPanel, 0, 0, 1, 1);
    layout->addWidget(splitter, 1, 0, 1, 1);
    
    splitter->addWidget(logArea);
    splitter->addWidget(props);
    
    props->verticalHeader()->hide();
    props->horizontalHeader()->setStretchLastSection(true);
    props->setCornerButtonEnabled(false);
    props->setShowGrid(false);

    connectBtn->setToolTip(tr("Connect"));
    connectBtn->setEnabled(false);
    launchBtn->setToolTip(tr("Launch"));
    launchBtn->setEnabled(false);
    removeBtn->setToolTip(tr("Remove"));
    removeBtn->setEnabled(false);
    QObject::connect(connectBtn, SIGNAL(clicked()), this, SLOT(onConnectClick()));
    QObject::connect(launchBtn, SIGNAL(clicked()), this, SLOT(onLaunchClick()));
    QObject::connect(removeBtn, SIGNAL(clicked()), this, SLOT(onRemoveClick()));
    
    topPanel->addWidget(connectBtn);
    topPanel->addWidget(launchBtn);
    topPanel->addStretch();
    topPanel->addWidget(removeBtn);
    
    layout->setContentsMargins(0,0,0,0);
}

void DetailedView::appendMessage(const QString& msg)
{
    logArea->appendPlainText(msg);
}

void DetailedView::clear()
{
    logArea->clear();
    connectBtn->setToolTip(tr("Connect"));
    connectBtn->setEnabled(false);
    connectBtn->setIcon(QIcon::fromTheme("state-ok"));
    launchBtn->setToolTip(tr("Launch"));
    launchBtn->setEnabled(false);
    launchBtn->setIcon(QIcon::fromTheme("kt-start"));
    removeBtn->setEnabled(false);
    connected = false;
    launched = false;
}

void DetailedView::setConnectable(bool value)
{
    connectBtn->setEnabled(value);
}

void DetailedView::setConnected(bool value)
{
    if (connected != value) {
        connected = value;
        if (connected) {
            connectBtn->setToolTip(tr("Disonnect"));
            connectBtn->setIcon(QIcon::fromTheme("state-error"));
        } else {
            connectBtn->setToolTip(tr("Connect"));
            connectBtn->setIcon(QIcon::fromTheme("state-ok"));
        }
    }
}

void DetailedView::setLaunchable(bool value)
{
    launchBtn->setEnabled(value);
}

void DetailedView::setLaunched(bool value)
{
    if (launched != value) {
        launched = value;
        if (launched) {
            launchBtn->setToolTip(tr("Stop"));
            launchBtn->setIcon(QIcon::fromTheme("kt-stop"));
        } else {
            launchBtn->setToolTip(tr("Launch"));
            launchBtn->setIcon(QIcon::fromTheme("kt-start"));
        }
    }
}

void DetailedView::onConnectClick()
{
    if (model == 0) {
        return;
    }
    if (connected) {
        emit disconnect(model->id);
    } else {
        emit connect(model->id);
    }
}

void DetailedView::onLaunchClick()
{
    if (model == 0) {
        return;
    }
    if (launched) {
        emit stop(model->id);
    } else {
        emit launch(model->id);
    }
}


void DetailedView::onRemoveClick()
{
    if (model == 0) {
        return;
    }
    emit remove(model->id);
}

void DetailedView::setRemovable(bool value)
{
    removeBtn->setEnabled(value);
}

void DetailedView::setModel(AppModel* p_model)
{
    if (model != 0) {
        clearModel();
    }
    model = p_model;
    QString* history = model->getHistory();
    appendMessage(*history);
    setConnectable(model->getConnectable());
    setConnected(model->getConnected());
    setLaunchable(model->getLaunchable());
    setLaunched(model->getLaunched());
    setRemovable(model->id != 0);
    delete history;
    QObject::connect(model, SIGNAL(newLogMessage(const QString&)), this, SLOT(appendMessage(const QString&)));
    QObject::connect(model, SIGNAL(changedConnectable(bool)), this, SLOT(setConnectable(bool)));
    QObject::connect(model, SIGNAL(changedConnected(bool)), this, SLOT(setConnected(bool)));
    QObject::connect(model, SIGNAL(changedLaunchable(bool)), this, SLOT(setLaunchable(bool)));
    QObject::connect(model, SIGNAL(changedLaunched(bool)), this, SLOT(setLaunched(bool)));
    
    QItemSelectionModel *m = props->selectionModel();
    props->setModel(&model->props);
    delete m;
}

void DetailedView::clearModel()
{
    if (model != 0) {
        clear();
        QObject::disconnect(model, SIGNAL(newLogMessage(const QString&)), this, SLOT(appendMessage(const QString&)));
        QObject::disconnect(model, SIGNAL(changedConnectable(bool)), this, SLOT(setConnectable(bool)));
        QObject::disconnect(model, SIGNAL(changedConnected(bool)), this, SLOT(setConnected(bool)));
        QObject::disconnect(model, SIGNAL(changedLaunchable(bool)), this, SLOT(setLaunchable(bool)));
        QObject::disconnect(model, SIGNAL(changedLaunched(bool)), this, SLOT(setLaunched(bool)));

        model = 0;
    }
}

void DetailedView::saveSettings()
{
    QSettings settings;
    
    settings.beginGroup("detailedView");
    
    settings.setValue("splitterState", splitter->saveState());
    settings.setValue("propsHeaderState", props->horizontalHeader()->saveState());
    
    settings.endGroup();
}

void DetailedView::readSettings()
{
    QSettings settings;
    
    splitter->restoreState(settings.value("detailedView/splitterState").toByteArray());
    props->horizontalHeader()->restoreState(settings.value("detailedView/propsHeaderState").toByteArray());
}

