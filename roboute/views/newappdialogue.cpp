#include "newappdialogue.h"

NewAppDialogue::NewAppDialogue(QWidget* parent):
    QDialog(parent),
    name(new QLineEdit(this)),
    address(new QLineEdit(this)),
    port(new QLineEdit(this)),
    login(new QLineEdit(this)),
    pass(new QLineEdit(this)),
    log(new QLineEdit(this)),
    command(new QLineEdit(this))
{
    construct();
}

NewAppDialogue::NewAppDialogue(const QMap<QString, QString>& data, QWidget* parent):
    QDialog(parent),
    name(new QLineEdit(this)),
    address(new QLineEdit(this)),
    port(new QLineEdit(this)),
    login(new QLineEdit(this)),
    pass(new QLineEdit(this)),
    log(new QLineEdit(this)),
    command(new QLineEdit(this))
{
    construct();
    
    if (data.contains("name")) {
        name->setText(data.value("name"));
    }
    
    if (data.contains("address")) {
        address->setText(data.value("address"));
    }
    
    if (data.contains("port")) {
        port->setText(data.value("port"));
    }
    
    if (data.contains("login")) {
        login->setText(data.value("login"));
    }
    
    if (data.contains("password")) {
        pass->setText(data.value("password"));
    }
    
    if (data.contains("logFile")) {
        log->setText(data.value("logFile"));
    }
    
    if (data.contains("command")) {
        command->setText(data.value("command"));
    }
}

void NewAppDialogue::construct()
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
    pass->setEchoMode(QLineEdit::Password);
    
    layout->addRow(tr("Name"), name);
    layout->addRow(tr("Server address"), address);
    layout->addRow(tr("Service port"), port);
    layout->addRow(tr("ssh login"), login);
    layout->addRow(tr("Password"), pass);
    layout->addRow(tr("Log file"), log);
    layout->addRow(tr("Command"), command);
}

QMap<QString, QString> NewAppDialogue::getData() const
{
    QMap<QString, QString> map;
    map.insert("name", name->text());
    map.insert("address", address->text());
    map.insert("port", port->text());
    map.insert("login", login->text());
    map.insert("password", pass->text());
    map.insert("logFile", log->text());
    map.insert("command", command->text());
    
    return map;
}

