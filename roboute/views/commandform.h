#ifndef COMMANDFORM_H
#define COMMANDFORM_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtCore/QVariant>
#include <QtCore/QMap>

class CommandForm : public QDialog
{
    Q_OBJECT
    typedef QMap<QString, uint64_t> AMap;
public:
    CommandForm(const QString& commandName, const AMap& p_args, QWidget* parent = 0);
    
    QMap<QString, QVariant> getData() const;
    QString getName() const;
    
private:
    AMap args;
    QMap<QString, QWidget*> editors;
    QString name;
    
    void createForm(QFormLayout* layout);
    void createButtons(QHBoxLayout* layout);
    
};

#endif // COMMANDFORM_H
