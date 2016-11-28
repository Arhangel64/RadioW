#ifndef DETAILEDVIEW_H
#define DETAILEDVIEW_H

#include "../models/appmodel.h"

#include <QtCore/QSettings>

#include <QtWidgets/QWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTableView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

class DetailedView : public QWidget
{
    Q_OBJECT
    
public:
    DetailedView(QWidget* parent = 0);
    void setModel(AppModel* p_model);
    void clearModel();
    
    void saveSettings();
    void readSettings();
    
private:
    QGridLayout* layout;
    QHBoxLayout* topPanel;
    QPlainTextEdit* logArea;
    QSplitter* splitter;
    QTableView* props;
    QPushButton* connectBtn;
    QPushButton* launchBtn;
    QPushButton* removeBtn;
    
    bool connected;
    bool launched;
    
    AppModel* model;
    
public slots:
    void appendMessage(const QString& msg);
    void clear();
    void setConnectable(bool value);
    void setConnected(bool value);
    void setLaunchable(bool value);
    void setLaunched(bool value);
    void setRemovable(bool value);
    
signals:
    void connect(uint64_t id);
    void disconnect(uint64_t id);
    void launch(uint64_t id);
    void stop(uint64_t id);
    void remove(uint64_t id);
    
private slots:
    void onConnectClick();
    void onLaunchClick();
    void onRemoveClick();
};

#endif // DETAILEDVIEW_H
