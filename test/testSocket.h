#ifndef TESTSOCKET_H
#define TESTSOCKET_H

#include <cxxtest/TestSuite.h>

#include <QtCore/QCoreApplication>

#include <wSocket/socket.h>
#include <wSocket/server.h>

class TestServer : public QObject
    {
        Q_OBJECT
    public:
        TestServer(QObject* parent):
            QObject(parent),
            server(new W::Server(W::String(u"test_server"), this))
        {
            connect(server, SIGNAL(newConnection(const W::Socket&)), SLOT(onNewConnection(const W::Socket&)));
            server->listen(8080);
        }
        
    private:
        W::Server *server;
        
    signals:
        void success();
        
    private slots:
        void onNewConnection(const W::Socket& socket)
        {
            connect(&socket, SIGNAL(message(const W::Event&)), SLOT(onSocketMessage(const W::Event&)));
        }
        
        void onSocketMessage(const W::Event& event)
        {
            W::Socket* socket = static_cast<W::Socket*>(sender());
            W::Address addr({socket->getName()});
            
            const W::String& msg = static_cast<const W::String&>(event.getData());
            
            TS_ASSERT_EQUALS(addr, event.getDestination());
            TS_ASSERT_EQUALS(msg, u"Hello, dear test server!");
            emit success();
        }
    };
    
    class TestClient : public QObject
    {
        Q_OBJECT
    public:
        TestClient(QObject* parent):
            QObject(parent),
            socket(new W::Socket(W::String(u"test_client"), this))
        {
            connect(socket, SIGNAL(connected()), SLOT(onConnected()));
            socket->open(W::String(u"localhost"), W::Uint64(8080));
        }
        
    private:
        W::Socket *socket;
        
    private slots:
        void onConnected()
        {
            W::Address addr({socket->getRemoteName()});
            W::String message(u"Hello, dear test server!");
            W::Event ev(addr, message);
            
            ev.setSenderId(socket->getId());
            
            socket->send(ev);
        }
    };

class TestSocket : public CxxTest::TestSuite
{
    
public:
    void testHandshake()
    {
        char a1[] = "nothing";
        char* argv[] = {a1};
        int   argc   = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
        QCoreApplication* app = new QCoreApplication(argc, argv);
    
        TestServer* srv = new TestServer(app);
        TestClient* cli = new TestClient(app);
        
        QObject::connect(srv, SIGNAL(success()), app, SLOT(quit()));
      
        app->exec();
        
        delete app;
    }
};

#endif //TESTSOCKET_H
