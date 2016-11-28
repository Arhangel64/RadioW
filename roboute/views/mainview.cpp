#include "mainview.h"

MainView::MainView(QAbstractListModel* model, QWidget* parent):
    QWidget(parent),
    splitter(new QSplitter(this)),
    list(new QListView(this)),
    details(new DetailedView(this)),
    detailed(false)
{
    QGridLayout* layout = new QGridLayout();
    setLayout(layout);
    
    //AppListItemDelegate* dlg = new AppListItemDelegate(this);
    //list->setItemDelegate(dlg);
    list->setModel(model);
    
    layout->addWidget(splitter, 0, 0, 1, 1);
    
    splitter->addWidget(list);
    splitter->addWidget(details);
    details->hide();
}

void MainView::hideDetails()
{
    if (detailed) {
        detailed = false;
        details->hide();
    }
}

void MainView::showDetails()
{
    if (!detailed) {
        detailed = true;
        details->show();
    }
}

void MainView::saveSettings()
{
    QSettings settings;
    settings.beginGroup("view");
    
    settings.setValue("splitterState", splitter->saveState());
    
    settings.endGroup();
    
    details->saveSettings();
}

void MainView::readSettings()
{
    QSettings settings;
    
    splitter->restoreState(settings.value("view/splitterState").toByteArray());
    
    details->readSettings();
}
