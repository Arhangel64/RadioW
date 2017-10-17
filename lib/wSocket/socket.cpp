#include "socket.h"

#include <iostream>

using std::cout;
using std::endl;

W::Socket::Socket(const W::String& p_name, QObject* parent): 
    QObject(parent),
    serverCreated(false),
    state(disconnected_s),
    socket(new QWebSocket()),
    id(0),
    name(p_name),
    remoteName()
{
    socket->setParent(this);
    setHandlers();
}

W::Socket::Socket(const W::String& p_name, QWebSocket* p_socket, uint64_t p_id, QObject* parent): 
    QObject(parent),
    serverCreated(true),
    state(disconnected_s),
    socket(p_socket),
    id(p_id),
    name(p_name),
    remoteName()
{
    socket->setParent(this);
    setHandlers();
}

W::Socket::~Socket()
{
    close();
}

void W::Socket::open(const W::String& addr, const W::Uint64& port)
{
    if (state == disconnected_s) {
        String::StdStr url_str("ws://" + addr.toString() + ":" + port.toString());
        QUrl url(url_str.c_str());
        state = connecting_s;
        socket->open(url);
    }
}

void W::Socket::close()
{
    if (state != disconnected_s && state != disconnecting_s) {
        state = disconnecting_s;
        socket->close();
    }
}

void W::Socket::send(const W::Event& ev) const
{
    //std::cout << "Sending event: " << ev.toString() << std::endl;
    ByteArray *wba = new ByteArray();
    *wba << ev;
    QByteArray *ba = WtoQ(*wba);
    delete wba;
    socket->sendBinaryMessage(*ba);
    delete ba;
}

W::Uint64 W::Socket::getId() const
{
    return id;
}

W::String W::Socket::getRemoteName() const
{
    return remoteName; //TODO may be throw the exception, when socket is not connected?
}

W::String W::Socket::getName() const
{
    return name;
}

void W::Socket::setHandlers() {
    connect(socket, SIGNAL(connected()), SLOT(onSocketConnected()));
    connect(socket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
    connect(socket, SIGNAL(binaryMessageReceived(const QByteArray&)), SLOT(onBinaryMessageReceived(const QByteArray&)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onSocketError(QAbstractSocket::SocketError)));
}

void W::Socket::onSocketConnected()
{
    
}

void W::Socket::onSocketDisconnected()
{
    state = disconnected_s;
    remoteName = String();
    emit disconnected();
}

void W::Socket::onBinaryMessageReceived(const QByteArray& ba)
{
    ByteArray *wba = QtoW(ba);
    Event *ev = static_cast<Event*>(Object::fromByteArray(*wba));
    delete wba;
    
    if (ev->isSystem()) {
        const Vocabulary& vc = static_cast<const Vocabulary&>(ev->getData());
        const String& command = static_cast<const String&>(vc.at(u"command"));
        
        if (command == u"setId") {
            if (serverCreated) {
                if (state == connecting_s) {
                    emit negotiationId(static_cast<const Uint64&>(vc.at(u"id")));
                } else {
                    throw ErrorIdSetting();
                }
            } else {
                setId(static_cast<const Uint64&>(vc.at(u"id")));
                setRemoteName();
            }
        } else if (command == u"setName") {
            setName(static_cast<const String&>(vc.at(u"name")));
            if (static_cast<const String&>(vc.at(u"yourName")) != name) {
                setRemoteName();
            }
            emit connected();
        }
    } else {
        emit message(*ev);
    }
    
    delete ev;
}

void W::Socket::setId(const W::Uint64& p_id)
{
    if (state == connecting_s)
    {
        id = p_id;
    }
    else
    {
        throw ErrorIdSetting();
    }
}

void W::Socket::setRemoteId()
{
    if (state == disconnected_s) {
        state = connecting_s;
    }
    String command(u"setId");
    Vocabulary *vc = new Vocabulary();
    
    vc->insert(u"command", command);
    vc->insert(u"id", id);
    
    Address addr;
    Event ev(addr, vc, true);
    ev.setSenderId(id);
    send(ev);
}

void W::Socket::setRemoteName()
{
    String command(u"setName");
    Vocabulary *vc = new Vocabulary();
    
    vc->insert(u"command", command);
    vc->insert(u"name", name);
    vc->insert(u"yourName", remoteName);
    
    Address addr;
    Event ev(addr, vc, true);
    ev.setSenderId(id);
    send(ev);
    
}

void W::Socket::setName(const W::String& p_name)
{
    if (state == connecting_s)
    {
        remoteName = p_name;
        state = connected_s;
    }
    else
    {
        throw ErrorNameSetting();
    }
}


W::ByteArray* W::Socket::QtoW(const QByteArray& in)
{
    ByteArray *out = new ByteArray();
    
    QByteArray::const_iterator itr = in.begin();
    QByteArray::const_iterator end = in.end();
    
    for (;itr != end; ++itr)
    {
        out->data->push_back(*itr);
    }
    
    return out;
}

QByteArray* W::Socket::WtoQ(const ByteArray& in)
{
    QByteArray *out = new QByteArray();
    
    ByteArray::Container::const_iterator itr = in.data->begin();
    ByteArray::Container::const_iterator end = in.data->end();
    
    for (;itr != end; ++itr)
    {
        out->push_back(*itr);
    }
    
    return out;
}

void W::Socket::cantDeliver(const W::Event& event) const
{
    String command(u"cantDeliver");
    Vocabulary *vc = new Vocabulary();
    
    vc->insert(u"command", command);
    vc->insert(u"event", event);
    
    Address addr;
    Event ev(addr, vc, true);
    ev.setSenderId(id);
    send(ev);
}

void W::Socket::onSocketError(QAbstractSocket::SocketError err)
{
    if (state == connecting_s) {
        state = disconnected_s;
    }
    //socket->close();
    emit error(err, socket->errorString());
}

