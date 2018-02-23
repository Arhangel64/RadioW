#include "corax.h"

#include <iostream>

using std::cout;
using std::endl;

Corax* Corax::corax = 0;

Corax::Corax(QObject *parent):
    QObject(parent),
    server(new W::Server(W::String(u"Corax"), this)),
    logger(new W::Logger()),
    parentReporter(new W::ParentReporter()),
    attributes(new M::Attributes(W::Address({u"attributes"}))),
    commands(new U::Commands(W::Address{u"management"})),
    connector(0),
    dispatcher(new W::Dispatcher()),
    caches(),
    parsers()
{
    if (corax != 0) 
    {
        throw SingletonError();
    }
    Corax::corax = this;
    
    connector = new U::Connector(dispatcher, server, commands);
    connector->addIgnoredNode(W::String(u"Lorgar"));
    connector->addIgnoredNode(W::String(u"Roboute"));
    
    connect(attributes, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(commands, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connector, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connector, SIGNAL(nodeConnected(const W::String&)), SLOT(onNodeConnected(const W::String&)));
    connect(connector, SIGNAL(nodeDisconnected(const W::String&)), SLOT(onNodeDisconnected(const W::String&)));
    connect(server, SIGNAL(connectionCountChange(uint64_t)), SLOT(onConnectionCountChanged(uint64_t)));
    
    dispatcher->registerDefaultHandler(parentReporter);
    dispatcher->registerDefaultHandler(logger);
    
    attributes->addAttribute(W::String(u"connectionsCount"), new M::String(W::String(u"0"), W::Address({u"attributes", u"connectionCount"})));
    attributes->addAttribute(W::String(u"name"), new M::String(W::String(u"Corax"), W::Address({u"attributes", u"name"})));
    attributes->addAttribute(W::String(u"version"), new M::String(W::String(u"0.0.1"), W::Address({u"attributes", u"version"})));
    
    createCaches();
    createHandlers();
}

Corax::~Corax()
{
    std::map<W::String, Parser*>::iterator pbeg = parsers.begin();
    std::map<W::String, Parser*>::iterator pend = parsers.end();
    
    for (; pbeg != pend; ++pbeg) {
        delete pbeg->second;
    }
    
    std::map<W::String, ResourceCache*>::iterator beg = caches.begin();
    std::map<W::String, ResourceCache*>::iterator end = caches.end();
    
    for (; beg != end; ++beg) {
        delete beg->second;
    }
    
    delete connector;
    
    dispatcher->unregisterDefaultHandler(logger);
    
    delete commands;
    delete attributes;
    
    delete logger;
    delete dispatcher;
    
    Corax::corax = 0;
}

void Corax::onConnectionCountChanged(uint64_t count)
{
    attributes->setAttribute(W::String(u"connectionsCount"), new W::String(std::to_string(count)));
}

void Corax::start()
{
    std::map<W::String, ResourceCache*>::iterator beg = caches.begin();
    std::map<W::String, ResourceCache*>::iterator end = caches.end();
    
    cout << "Starting corax..." << endl;
    server->listen(8080);
    
    cout << "Registering models..." << endl;
    attributes->registerModel(dispatcher, server);
    commands->registerModel(dispatcher, server);
    
    for (; beg != end; ++beg) {
        beg->second->registerModel(dispatcher, server);
    }
    
    cout << "Opening caches..." << endl;
    
    beg = caches.begin();
    for (; beg != end; ++beg) {
        beg->second->open();
    }
    
    commands->enableCommand(W::String(u"clearCache"), true);
    
    cout << "Corax is ready" << endl;
}

void Corax::stop()
{
    std::map<W::String, ResourceCache*>::iterator beg = caches.begin();
    std::map<W::String, ResourceCache*>::iterator end = caches.end();
    
    cout << "Stopping corax..." << endl;
    commands->unregisterModel();
    attributes->unregisterModel();
    
    for (; beg != end; ++beg) {
        beg->second->unregisterModel();
    }
    
    server->stop();
}

void Corax::onModelServiceMessage(const QString& msg)
{
    cout << msg.toStdString() << endl;
}

void Corax::addCache(ResourceCache* cache)
{
    attributes->addAttribute(cache->name, new M::String(W::String(u"0"), W::Address({u"attributes", cache->name})));
    
    connect(cache, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(cache, SIGNAL(countChange(uint64_t)), SLOT(onCacheCountChange(uint64_t)));
    
    parentReporter->registerParent(cache->getAddress(), cache->subscribeMember);
    
    caches.insert(std::make_pair(cache->name, cache));
}

void Corax::h_clearCache(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::String& name = static_cast<const W::String&>(vc.at(u"name"));
    
    cout << "received command to clear cache " << name.toString() << endl;
    
    std::map<W::String, ResourceCache*>::iterator itr = caches.find(name);
    if (itr == caches.end()) {
        cout << "cache " << name.toString() << " doesn't exist" << endl;
    } else {
        itr->second->clear();
    }
}

void Corax::h_parseDirectory(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::String& path = static_cast<const W::String&>(vc.at(u"path"));
    
    cout << "received command to parse directory " << path.toString() << endl;
    
    std::map<W::String, Parser*>::const_iterator itr = parsers.find(path);
    
    if (itr != parsers.end()) {
        cout << "directory " << path.toString() << " is already being parsed" << endl;
    } else {
        const W::Socket& socket = connector->getNodeSocket(W::String(u"Perturabo"));
        ResourceCache* music = caches.at(W::String(u"music"));
        ResourceCache* images = caches.at(W::String(u"images"));
        Parser* parser = new Parser(&socket, dispatcher, music, images);
        parsers.insert(std::make_pair(path, parser));
        
        connect(parser, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
        connect(parser, SIGNAL(done(const W::String&)), SLOT(onParserDone(const W::String&)));
        
        parser->parse(path);
    }
}

void Corax::createCaches()
{
    ResourceCache* music = new ResourceCache(W::String(u"music"));
    ResourceCache* images = new ResourceCache(W::String(u"images"));
    
    addCache(music);
    addCache(images);
}

void Corax::createHandlers()
{
    W::Handler* clearCache = W::Handler::create(W::Address({u"management", u"clearCache"}), this, &Corax::_h_clearCache);
    W::Vocabulary clearArgs;
    clearArgs.insert(u"name", W::Uint64(W::Object::string));
    commands->addCommand(W::String(u"clearCache"), clearCache, clearArgs);
    
    W::Handler* parseDirectory = W::Handler::create(W::Address({u"management", u"parseDirectory"}), this, &Corax::_h_parseDirectory);
    W::Vocabulary parseArgs;
    parseArgs.insert(u"path", W::Uint64(W::Object::string));
    commands->addCommand(W::String(u"parseDirectory"), parseDirectory, parseArgs);
}

void Corax::onParserDone(const W::String& path)
{
    std::map<W::String, Parser*>::const_iterator itr = parsers.find(path);
    
    delete itr->second;
    parsers.erase(itr);
}

void Corax::onCacheCountChange(uint64_t count)
{
    ResourceCache* cache = static_cast<ResourceCache*>(sender());
    
    attributes->setAttribute(cache->name, W::String(std::to_string(count)));
}

void Corax::onNodeConnected(const W::String& name)
{
    cout << "connected node " << name.toString() << endl;
    if (name == u"Perturabo") {
        commands->enableCommand(W::String(u"parseDirectory"), true);
    }
}

void Corax::onNodeDisconnected(const W::String& name)
{
    cout << "disconnected node " << name.toString() << endl;
    if (name == u"Perturabo") {
        commands->enableCommand(W::String(u"parseDirectory"), false);
    }
}
