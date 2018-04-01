#include "socket.h"

#include <iostream>

using std::cout;
using std::endl;

W::Socket::Socket(const W::String& p_name, QObject* parent): 
    QObject(parent),
    serverCreated(false),
    state(disconnected_s),
    dState(dSize),
    socket(new QWebSocket()),
    id(0),
    name(p_name),
    remoteName(),
    helperBuffer(new W::ByteArray(4))
{
    socket->setParent(this);
    setHandlers();
}

W::Socket::Socket(const W::String& p_name, QWebSocket* p_socket, uint64_t p_id, QObject* parent): 
    QObject(parent),
    serverCreated(true),
    state(disconnected_s),
    dState(dSize),
    socket(p_socket),
    id(p_id),
    name(p_name),
    remoteName(),
    helperBuffer(new W::ByteArray(4))
{
    socket->setParent(this);
    setHandlers();
}

W::Socket::~Socket()
{
    close();
    
    delete helperBuffer;
}

void W::Socket::open(const W::String& addr, const W::Uint64& port)
{
    if (state == disconnected_s) {
        String::StdStr url_str("ws://" + addr.toString() + ":" + port.toString());
        QUrl url(url_str.c_str());
        remoteName = String();
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
    W::Object::size_type size = ev.size();
    ByteArray *wba = new ByteArray(size + 5);
    wba->push32(size);
    wba->push8(ev.getType());
    ev.serialize(*wba);
    
    QByteArray ba = QByteArray::fromRawData(wba->getData(), wba->size());
    socket->sendBinaryMessage(ba);
    delete wba;
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
    dState = dSize;
    delete helperBuffer;
    helperBuffer = new W::ByteArray(4);
}

void W::Socket::onSocketDisconnected()
{
    state = disconnected_s;
    emit disconnected();
}

void W::Socket::onBinaryMessageReceived(const QByteArray& ba)
{
    int i = 0;
    while (i < ba.size()) {
        switch (dState) {
            case dSize:
                i = helperBuffer->fill(ba.data(), ba.size(), i);
                
                if (helperBuffer->filled()) {
                    int size = helperBuffer->pop32();
                    delete helperBuffer;
                    helperBuffer = new W::ByteArray(size + 1);
                    dState = dBody;
                }
                break;
            case dBody:
                i = helperBuffer->fill(ba.data(), ba.size(), i);
                
                if (helperBuffer->filled()) {
                    Event* ev = static_cast<Event*>(W::Object::fromByteArray(*helperBuffer));
                    onEvent(ev);
                    
                    delete ev;
                    
                    delete helperBuffer;
                    helperBuffer = new W::ByteArray(4);
                    dState = dSize;
                }
                break;
        }
    }
}

void W::Socket::onEvent(W::Event* ev)
{
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

