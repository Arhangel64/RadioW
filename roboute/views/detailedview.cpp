#include "detailedview.h"

DetailedView::DetailedView(QWidget* parent):
    QWidget(parent),
    layout(new QGridLayout(this)),
    topPanel(new QHBoxLayout()),
    logArea(new QPlainTextEdit(this)),
    connectBtn(new QPushButton(QIcon::fromTheme("state-error"), "", this)),
    launchBtn(new QPushButton(QIcon::fromTheme("kt-start"), "", this)),
    connected(false),
    launched(false)
{
    setLayout(layout);
    logArea->setReadOnly(true);
    
    layout->addLayout(topPanel, 0, 0, 1, 1);
    layout->addWidget(logArea, 1, 0, 1, 1);

    connectBtn->setToolTip(tr("Connect"));
    connectBtn->setEnabled(false);
    launchBtn->setToolTip(tr("Launch"));
    launchBtn->setEnabled(false);
    QObject::connect(connectBtn, SIGNAL(clicked()), this, SLOT(onConnectClick()));
    QObject::connect(launchBtn, SIGNAL(clicked()), this, SLOT(onLaunchClick()));
    
    topPanel->addWidget(connectBtn);
    topPanel->addWidget(launchBtn);
    topPanel->addStretch();
    
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
    connectBtn->setIcon(QIcon::fromTheme("state-error"));
    launchBtn->setToolTip(tr("Launch"));
    launchBtn->setEnabled(false);
    launchBtn->setIcon(QIcon::fromTheme("kt-start"));
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
            connectBtn->setIcon(QIcon::fromTheme("state-ok"));
        } else {
            connectBtn->setToolTip(tr("Connect"));
            connectBtn->setIcon(QIcon::fromTheme("state-error"));
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
    if (connected) {
        emit disconnect();
    } else {
        emit connect();
    }
}

void DetailedView::onLaunchClick()
{
    if (launched) {
        emit stop();
    } else {
        emit launch();
    }
}
