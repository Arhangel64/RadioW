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
    artists(new Database(W::String(u"artists"))),
    albums(new Database(W::String(u"albums"))),
    songs(new Database(W::String(u"songs"))),
    dispatcher(new W::Dispatcher())
{
    if (perturabo != 0) 
    {
        throw SingletonError();
    }
    Perturabo::perturabo = this;
    
    connector = new U::Connector(W::String(u"Perturabo"), dispatcher, server, commands);
    W::Handler* parseDirectory = W::Handler::create(W::Address({u"management", u"parseDirectory"}), this, &Perturabo::_h_parseDirectory);
    
    connect(attributes, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(commands, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connector, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(artists, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(albums, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(songs, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    
    connect(connector, SIGNAL(connectionCountChange(uint64_t)), SLOT(onConnectionCountChanged(uint64_t)));
    
    parentReporter->registerParent(artists->getAddress(), artists->subscribeMember);
    parentReporter->registerParent(albums->getAddress(), albums->subscribeMember);
    parentReporter->registerParent(songs->getAddress(), songs->subscribeMember);
    
    dispatcher->registerDefaultHandler(parentReporter);
    dispatcher->registerDefaultHandler(logger);
    
    connector->addNode(W::String(u"Magnus"));
    connector->addNode(W::String(u"Corax"));
    
    attributes->addAttribute(W::String(u"connectionsCount"), new M::String(W::String(u"0"), W::Address({u"attributes", u"connectionCount"})));
    attributes->addAttribute(W::String(u"name"), new M::String(W::String(u"Perturabo"), W::Address({u"attributes", u"name"})));
    attributes->addAttribute(W::String(u"version"), new M::String(W::String(u"0.0.1"), W::Address({u"attributes", u"version"})));
    
    artists->addIndex(W::String(u"name"), W::Object::string);
    
    albums->addIndex(W::String(u"name"), W::Object::string);
    albums->addIndex(W::String(u"artist"), W::Object::uint64);
    
    songs->addIndex(W::String(u"name"), W::Object::string);
    songs->addIndex(W::String(u"artist"), W::Object::uint64);
    songs->addIndex(W::String(u"album"), W::Object::uint64);
    
    W::Vocabulary args;
    args.insert(u"path", W::Uint64(W::Object::string));
    commands->addCommand(W::String(u"parseDirectory"), parseDirectory, args);
}

Perturabo::~Perturabo()
{
    delete songs;
    delete albums;
    delete artists;
    
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
    cout << "Starting perturabo..." << endl;
    server->listen(8082);
    
    cout << "Registering models..." << endl;
    attributes->registerModel(dispatcher, server);
    commands->registerModel(dispatcher, server);
    artists->registerModel(dispatcher, server);
    albums->registerModel(dispatcher, server);
    songs->registerModel(dispatcher, server);
    
    cout << "Opening and indexing databases..." << endl;
    artists->open();
    albums->open();
    songs->open();
    
    commands->enableCommand(W::String(u"parseDirectory"), true);
    
    cout << "Perturabo is ready" << endl;
}

void Perturabo::stop()
{
    cout << "Stopping perturabo..." << endl;
    
    commands->enableCommand(W::String(u"parseDirectory"), false);
    
    songs->unregisterModel();
    albums->unregisterModel();
    artists->unregisterModel();
    
    commands->unregisterModel();
    attributes->unregisterModel();
    server->stop();
}

void Perturabo::onModelServiceMessage(const QString& msg)
{
    cout << msg.toStdString() << endl;
}

void Perturabo::h_parseDirectory(const W::Event& ev)
{
    commands->enableCommand(W::String(u"parseDirectory"), false);
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::String& path = static_cast<const W::String&>(vc.at(u"path"));
    
    cout << "Starting to parse directory " << path.toString() << endl;
    
    std::list<W::File> *list = new std::list<W::File>();
    bool success = W::File::readDirectoryRecursive(path, list);
    int songsAdded(0);
    int albumsAdded(0);
    int artistsAdded(0);
    
    if (success) {
        std::list<W::File>::const_iterator itr = list->begin();
        std::list<W::File>::const_iterator end = list->end();
        int i = 0;
        int total = list->size();
        
        for (; itr != end; ++itr) {
            if (itr->suffix() == u"mp3") {
                AudioTag tag(*itr);
                const std::set<uint64_t>& art = artists->find(W::String(u"name"), tag.getArtist());
                uint64_t artistId;
                if (art.size() == 0) {
                    W::Vocabulary avc;
                    avc.insert(u"name", tag.getArtist());
                    artistId = artists->addElement(avc);
                    ++artistsAdded;
                } else {
                    artistId = *(art.begin());
                }
                
                W::Vocabulary albvc;
                albvc.insert(u"name", tag.getAlbum());
                albvc.insert(u"artist", W::Uint64(artistId));
                std::set<uint64_t> alb = albums->find(albvc);
                uint64_t albumId;
                if (alb.size() == 0) {
                    albumId = albums->addElement(albvc);
                    ++albumsAdded;
                } else {
                    albumId = *(alb.begin());
                }
                
                W::Vocabulary svc;
                svc.insert(u"name", tag.getTitle());
                svc.insert(u"artist", W::Uint64(artistId));
                svc.insert(u"album", W::Uint64(albumId));
                
                std::set<uint64_t> sng = songs->find(svc);
                if (sng.size() == 0) {
                    songs->addElement(svc);
                    ++songsAdded;
                }
            }
            ++i;
            cout << "Finished " << i << " out of " << total << endl;
        }
    } else {
        cout << "Error: a problem with reading directory" << endl;
    }
    
    cout << "Parsing complete!" << endl;
    cout << artistsAdded << " artists added" << endl;
    cout << albumsAdded << " albums added" << endl;
    cout << songsAdded << " songs added" << endl;
    
    delete list;
    
    commands->enableCommand(W::String(u"parseDirectory"), true);
}

