#ifndef NEWAPPDIALOGUE_H
#define NEWAPPDIALOGUE_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QLineEdit>
#include <QtCore/QMap>

class NewAppDialogue : public QDialog
{
    Q_OBJECT
public:
    NewAppDialogue(QWidget* parent = 0);
    
    QMap<QString, QString> getData() const;
    
private:
    QLineEdit* name;
    QLineEdit* address;
    QLineEdit* port;
    
    
private:
    void createButtons(QHBoxLayout* layout);
    void createForm(QFormLayout* layout);
};

#endif // NEWAPPDIALOGUE_H
