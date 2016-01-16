#ifndef TESTDISPATCHER_H
#define TESTDISPATCHER_H

#include <cxxtest/TestSuite.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>

#include <wDispatcher/dispatcher.h>
#include <wDispatcher/handler.h>
#include <wType/event.h>

class TestObject: public QObject
{
    Q_OBJECT
    
public:
    TestObject(W::String hop, W::Dispatcher* p_rdp):
        QObject(),
        dp(new W::Dispatcher(W::Address({hop}), this)),
        right_h(0),
        wrong_h(0),
        root_dp(p_rdp)
    {
        right_h = W::Handler::create(W::Address({U"right"}), this, &TestObject::right);
        wrong_h = W::Handler::create(W::Address({U"wrong"}), this, &TestObject::wrong);
        
        dp->registerHandler(right_h);
        dp->registerHandler(wrong_h);
    }
    
    ~TestObject() 
    {
        dp->unregisterHandler(right_h);
        dp->unregisterHandler(wrong_h);
        
        delete right_h;
        delete wrong_h;
        
        delete dp;
    }
    
    void right(const W::Event& ev)
    {
        emit success();
    }
    
    void wrong(const W::Event& ev)
    {
        
    }
    
    W::Dispatcher* dp;
    W::Handler* right_h;
    W::Handler* wrong_h;
    W::Dispatcher* root_dp;
    
signals:
    void success();
    
public slots:
    void launch()
    {
        W::Event ev(W::Address({U"client", U"123", U"some_hop", U"main", U"right"}), W::String(U"hello!"));
        
        root_dp->pass(ev);
    }
    
};

class TestDispatcher : public CxxTest::TestSuite
{
    
public:
    void testEventPassing()
    {
        char a1[] = "nothing";
        char* argv[] = {a1};
        int   argc   = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
        QCoreApplication app (argc, argv);
        
        W::Dispatcher *root_dp = new W::Dispatcher(W::Address({U"client", U"123"}));
        W::Dispatcher *hop_dp = new W::Dispatcher(W::Address({U"some_hop"}));
        TestObject *test_object = new TestObject(W::String(U"main"), root_dp);
        
        root_dp->registerDispatcher(hop_dp);
        hop_dp->registerDispatcher(test_object->dp);
        
        QObject::connect(test_object, SIGNAL(success()), &app, SLOT(quit()));
        
        QTimer *timer = new QTimer(&app);
        QObject::connect(timer, SIGNAL(timeout()), test_object, SLOT(launch()));
        timer->start(0);
        
        app.exec();
        
        delete root_dp;
        delete hop_dp;
        delete test_object;
    }
};

#endif //TESTDISPATCHER_H