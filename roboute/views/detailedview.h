#ifndef DETAILEDVIEW_H
#define DETAILEDVIEW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QGridLayout>

class DetailedView : public QWidget
{
    Q_OBJECT
    
public:
    DetailedView(QWidget* parent = 0);
    
private:
    QGridLayout* layout;
    QPlainTextEdit* logArea;
    
public slots:
    void appendMessage(const QString& msg);
    void clear();
    
};

#endif // DETAILEDVIEW_H
