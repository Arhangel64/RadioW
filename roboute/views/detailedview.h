#ifndef DETAILEDVIEW_H
#define DETAILEDVIEW_H

#include "../models/appmodel.h"

#include <QtCore/QSettings>

#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTableView>
#include <QtWidgets/QListView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDockWidget>

class DetailedView : public QWidget
{
    Q_OBJECT
    
public:
    DetailedView(QWidget* parent = 0);
    void setModel(AppModel* p_model);
    void clearModel();
    uint64_t getModelId();
    
    void saveSettings();
    void readSettings();
    
    void showAttrs(bool value);
    void showCommands(bool value);
    
private:
    QGridLayout* layout;
    QHBoxLayout* topPanel;
    QTextEdit* logArea;
    QSplitter* splitter;
    QWidget* dock;
    QTableView* props;
    QListView* commands;
    QPushButton* connectBtn;
    QPushButton* launchBtn;
    QPushButton* clearBtn;
    QPushButton* removeBtn;
    QPushButton* editBtn;
    
    bool connected;
    bool launched;
    bool propsShown;
    bool commandsShown;
    
    AppModel* model;
    
public slots:
    void appendMessage(const QString& msg);
    void clear();
    void setConnectable(bool value);
    void setConnected(bool value);
    void setLaunchable(bool value);
    void setLaunched(bool value);
    void setRemovable(bool value);
    void setEditable(bool value);
    void clearedLog();
    
signals:
    void connect(uint64_t id);
    void disconnect(uint64_t id);
    void launch(uint64_t id);
    void stop(uint64_t id);
    void remove(uint64_t id);
    void edit(uint64_t id);
    void launchCommand(uint64_t id, const QString& name);
    void clearLog(uint64_t id);
    
private slots:
    void onConnectClick();
    void onLaunchClick();
    void onClearClick();
    void onRemoveClick();
    void onEditClick();
    void checkDock();
    void onCommandDoubleClicked(const QModelIndex& index);
};

#endif // DETAILEDVIEW_H
