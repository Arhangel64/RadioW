#include "socket.h"

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

W::Socket::Socket(const W::String& p_name, QWebSocket* p_socket, QObject* parent): 
    QObject(parent),
    serverCreated(true),
    state(connecting_s),
    socket(p_socket),
    id(0),
    name(p_name),
    remoteName()
{
    socket->setParent(this);
    setHandlers();
}

W::Socket::~Socket()
{

}

void W::Socket::send(W::Event ev) const
{
    ev.setSenderId(id);
    ByteArray *wba;
    *wba << ev;
    QByteArray *ba = WtoQ(*wba);
    delete wba;
    socket->sendBinaryMessage(*ba);
    delete ba;
}

void W::Socket::setHandlers() {
    connect(socket, SIGNAL(connected()), SLOT(onSocketConnected()));
    connect(socket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
    connect(socket, SIGNAL(binaryMessageReceived(const QByteArray&)), SLOT(onBinaryMessageReceived(const QByteArray&)));
}

void W::Socket::onSocketConnected()
{

}

void W::Socket::onSocketDisconnected()
{
    id = W::Uint64(0);
    state = disconnected_s;
    emit disconnected();
}

void W::Socket::onBinaryMessageReceived(const QByteArray& ba)
{
    ByteArray *wba = QtoW(ba);
    Event *ev = static_cast<Event*>(Object::fromByteArray(*wba));
    delete wba;
    
    if (ev->isSystem())
    {
        const Vocabulary& vc = static_cast<const Vocabulary&>(ev->getData());
        const String& command = static_cast<const String&>(vc.at(U"command"));
        
        if (command == U"setId")
        {
            setId(static_cast<const Uint64&>(vc.at(U"id")));
            
        }
        else if (command == U"setRemoteName")
        {
            
        }
    }
    else
    {
        
    }
    
    delete ev;
}

void W::Socket::setId(const W::Uint64& p_id)
{
    if (id == 0 && state == connecting_s)
    {
        id = p_id;
        setRemoteName();
    }
    else
    {
        throw ErrorIdSetting();
    }
}

void W::Socket::setRemoteName()
{
    String command(U"setRemoteName");
    Vocabulary *vc;
    
    vc->insert(U"command", command);
    vc->insert(U"name", name);
    
    Address addr;
    Event ev(addr, vc, true);
    send(ev);
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

