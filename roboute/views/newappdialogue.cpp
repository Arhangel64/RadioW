#include "newappdialogue.h"

NewAppDialogue::NewAppDialogue(QWidget* parent):
    QDialog(parent),
    name(new QLineEdit(this)),
    address(new QLineEdit(this)),
    port(new QLineEdit(this))
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    QFormLayout* formLayout = new QFormLayout();
    
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);
    
    setLayout(mainLayout);
    
    createButtons(buttonsLayout);
    createForm(formLayout);
}

void NewAppDialogue::createButtons(QHBoxLayout* layout)
{
    layout->addStretch();
    
    QPushButton* accept = new QPushButton(QIcon::fromTheme("dialog-ok"), tr("OK"), this);
    QPushButton* reject = new QPushButton(QIcon::fromTheme("dialog-cancel"), tr("Cancel"), this);
    
    connect(accept, SIGNAL(clicked()), SLOT(accept()));
    connect(reject, SIGNAL(clicked()), SLOT(reject()));
    
    layout->addWidget(accept);
    layout->addWidget(reject);
}

void NewAppDialogue::createForm(QFormLayout* layout)
{
    layout->addRow(tr("Name"), name);
    layout->addRow(tr("Server address"), address);
    layout->addRow(tr("Service port"), port);
}

QMap<QString, QString> NewAppDialogue::getData() const
{
    QMap<QString, QString> map;
    map.insert("name", name->text());
    map.insert("address", address->text());
    map.insert("port", port->text());
    
    return map;
}

