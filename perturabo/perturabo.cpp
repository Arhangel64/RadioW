#include "perturabo.h"

#include <iostream>

using std::cout;
using std::endl;

Perturabo* Perturabo::perturabo = 0;

Perturabo::Perturabo(QObject *parent):
    QObject(parent),
    server(new W::Server(W::String(u"Perturabo"), this)),
    logger(new W::Logger()),
    parentReporter(new W::ParentReporter()),
    attributes(new M::Attributes(W::Address({u"attributes"}))),
    commands(new U::Commands(W::Address{u"management"})),
    connector(0),
    databases(),
    dispatcher(new W::Dispatcher())
{
    if (perturabo != 0) 
    {
        throw SingletonError();
    }
    Perturabo::perturabo = this;
    
    connector = new U::Connector(dispatcher, server, commands);
    connector->addIgnoredNode(W::String(u"Lorgar"));
    connector->addIgnoredNode(W::String(u"Roboute"));
    
    connect(attributes, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(commands, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connector, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(server, SIGNAL(connectionCountChange(uint64_t)), SLOT(onConnectionCountChanged(uint64_t)));
    
    dispatcher->registerDefaultHandler(parentReporter);
    dispatcher->registerDefaultHandler(logger);
    
    attributes->addAttribute(W::String(u"connectionsCount"), new M::String(W::String(u"0"), W::Address({u"attributes", u"connectionCount"})));
    attributes->addAttribute(W::String(u"name"), new M::String(W::String(u"Perturabo"), W::Address({u"attributes", u"name"})));
    attributes->addAttribute(W::String(u"version"), new M::String(W::String(u"0.0.3"), W::Address({u"attributes", u"version"})));
    
    createDatabases();
    
    W::Handler* clearDatabase = W::Handler::create(W::Address({u"management", u"clearDatabase"}), this, &Perturabo::_h_clearDatabase);
    W::Vocabulary clearArgs;
    clearArgs.insert(u"name", W::Uint64(W::Object::string));
    commands->addCommand(W::String(u"clearDatabase"), clearDatabase, clearArgs);
}

Perturabo::~Perturabo()
{
    std::map<W::String, Database*>::iterator beg = databases.begin();
    std::map<W::String, Database*>::iterator end = databases.end();
    
    for (; beg != end; ++beg) {
        delete beg->second;
    }
    
    delete connector;
    
    dispatcher->unregisterDefaultHandler(logger);
    dispatcher->unregisterDefaultHandler(parentReporter);
    
    delete commands;
    delete attributes;
    
    delete parentReporter;
    delete logger;
    delete dispatcher;
    
    Perturabo::perturabo = 0;
}

void Perturabo::onConnectionCountChanged(uint64_t count)
{
    attributes->setAttribute(W::String(u"connectionsCount"), new W::String(std::to_string(count)));
}

void Perturabo::start()
{
    std::map<W::String, Database*>::iterator beg = databases.begin();
    std::map<W::String, Database*>::iterator end = databases.end();
    
    cout << "Starting perturabo..." << endl;
    server->listen(8082);
    
    cout << "Registering models..." << endl;
    attributes->registerModel(dispatcher, server);
    commands->registerModel(dispatcher, server);
    
    for (; beg != end; ++beg) {
        beg->second->registerModel(dispatcher, server);
    }
    
    cout << "Opening and indexing databases..." << endl;
    
    beg = databases.begin();
    for (; beg != end; ++beg) {
        beg->second->open();
    }
    
    commands->enableCommand(W::String(u"clearDatabase"), true);
    
    cout << "Perturabo is ready" << endl;
}

void Perturabo::stop()
{
    std::map<W::String, Database*>::iterator beg = databases.begin();
    std::map<W::String, Database*>::iterator end = databases.end();
    
    cout << "Stopping perturabo..." << endl;
    
    commands->enableCommand(W::String(u"clearDatabase"), false);
    
    for (; beg != end; ++beg) {
        beg->second->unregisterModel();
    }
    
    commands->unregisterModel();
    attributes->unregisterModel();
    server->stop();
}

void Perturabo::onModelServiceMessage(const QString& msg)
{
    cout << msg.toStdString() << endl;
}

void Perturabo::h_clearDatabase(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::String& name = static_cast<const W::String&>(vc.at(u"name"));
 
    cout << "received command to clear database " << name.toString() << endl;
    
    std::map<W::String, Database*>::iterator itr = databases.find(name);
    if (itr == databases.end()) {
        cout << "database " << name.toString() << " doesn't exist" << endl;
    } else {
        itr->second->clear();
    }
}


void Perturabo::addDatabase(Database* db)
{
    connect(db, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    parentReporter->registerParent(db->getAddress(), db->subscribeMember);
    
    databases.insert(std::make_pair(db->name, db));
}

void Perturabo::createDatabases()
{
    Database* artists = new Database(W::String(u"artists"));
    Database* albums = new Database(W::String(u"albums"));
    Database* songs = new Database(W::String(u"songs"));
    
    artists->addIndex(W::String(u"name"), W::Object::string);
    
    albums->addIndex(W::String(u"name"), W::Object::string);
    albums->addIndex(W::String(u"artist"), W::Object::uint64);
    
    songs->addIndex(W::String(u"name"), W::Object::string);
    songs->addIndex(W::String(u"artist"), W::Object::uint64);
    songs->addIndex(W::String(u"album"), W::Object::uint64);
    
    attributes->addAttribute(W::String(artists->name), new M::String(W::String(u"0"), W::Address({u"attributes", artists->name})));
    attributes->addAttribute(W::String(albums->name), new M::String(W::String(u"0"), W::Address({u"attributes", albums->name})));
    attributes->addAttribute(W::String(songs->name), new M::String(W::String(u"0"), W::Address({u"attributes", songs->name})));
    
    connect(artists, SIGNAL(countChange(uint64_t)), SLOT(onDatabaseCountChange(uint64_t)));
    connect(albums, SIGNAL(countChange(uint64_t)), SLOT(onDatabaseCountChange(uint64_t)));
    connect(songs, SIGNAL(countChange(uint64_t)), SLOT(onDatabaseCountChange(uint64_t)));
    
    addDatabase(artists);
    addDatabase(albums);
    addDatabase(songs);
}

void Perturabo::onDatabaseCountChange(uint64_t count)
{
    Database* db = static_cast<Database*>(sender());
    
    attributes->setAttribute(db->name, W::String(std::to_string(count)));
}

