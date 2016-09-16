#ifndef DETAILEDVIEW_H
#define DETAILEDVIEW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

class DetailedView : public QWidget
{
    Q_OBJECT
    
public:
    DetailedView(QWidget* parent = 0);
    
private:
    QGridLayout* layout;
    QHBoxLayout* topPanel;
    QPlainTextEdit* logArea;
    QPushButton* connectBtn;
    QPushButton* launchBtn;
    QPushButton* removeBtn;
    
    bool connected;
    bool launched;
    
public slots:
    void appendMessage(const QString& msg);
    void clear();
    void setConnectable(bool value);
    void setConnected(bool value);
    void setLaunchable(bool value);
    void setLaunched(bool value);
    void setRemovable(bool value);
    
signals:
    void connect();
    void disconnect();
    void launch();
    void stop();
    void remove();
    
private slots:
    void onConnectClick();
    void onLaunchClick();
    void onRemoveClick();
};

#endif // DETAILEDVIEW_H
