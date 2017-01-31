#include "commandform.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>

CommandForm::CommandForm(const QString& commandName, const AMap& p_args, QWidget* parent):
    QDialog(parent),
    args(p_args),
    editors(),
    name(commandName)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    QFormLayout* formLayout = new QFormLayout();
    
    mainLayout->addWidget(new QLabel(name, this));
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);
    
    setLayout(mainLayout);
    createForm(formLayout);
    createButtons(buttonsLayout);
}

void CommandForm::createButtons(QHBoxLayout* layout)
{
    layout->addStretch();
    
    QPushButton* accept = new QPushButton(QIcon::fromTheme("dialog-ok"), tr("OK"), this);
    QPushButton* reject = new QPushButton(QIcon::fromTheme("dialog-cancel"), tr("Cancel"), this);
    
    connect(accept, SIGNAL(clicked()), SLOT(accept()));
    connect(reject, SIGNAL(clicked()), SLOT(reject()));
    
    layout->addWidget(accept);
    layout->addWidget(reject);
}

void CommandForm::createForm(QFormLayout* layout)
{
    AMap::const_iterator itr = args.begin();
    AMap::const_iterator end = args.end();
    
    for (; itr != end; ++itr) {
        bool supportable = false;
        QWidget* editor;
        
        switch (itr.value()) {
            case 0:
                editor = new QLineEdit(this);
                supportable = true;
                break;
            case 2:
                QSpinBox* spbox = new QSpinBox(this);
                spbox->setMaximum(UINT16_MAX);      //TODO what the hell is wrong with this shit?
                editor = spbox;
                supportable = true;
                break;
        }
        
        if (supportable) {
            layout->addRow(tr(itr.key().toStdString().c_str()), editor);
            editors.insert(itr.key(), editor);
        }
        
    }
}

QMap<QString, QVariant> CommandForm::getData() const
{
    AMap::const_iterator itr = args.begin();
    AMap::const_iterator end = args.end();
    
    QMap<QString, QVariant> result;
    
    for (; itr != end; ++itr) {
        bool supportable = false;
        QWidget* editor = editors.find(itr.key()).value();
        
        switch (itr.value()) {
            case 0:
                result.insert(itr.key(), static_cast<QLineEdit*>(editor)->text());
                supportable = true;
                break;
            case 2:
                result.insert(itr.key(), static_cast<QSpinBox*>(editor)->value());
                supportable = true;
                break;
        }
    }
    
    return result;
}

QString CommandForm::getName() const
{
    return name;
}



