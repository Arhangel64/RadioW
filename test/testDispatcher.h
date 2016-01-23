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
    TestObject(const W::Address& addr, W::Dispatcher* p_dp):
        QObject(),
        right_h(0),
        wrong_h(0),
        dp(p_dp)
    {
        right_h = W::Handler::create(addr + W::Address({u"right"}), this, &TestObject::right);
        wrong_h = W::Handler::create(addr + W::Address({u"wrong"}), this, &TestObject::wrong);
        
        dp->registerHandler(right_h);
        dp->registerHandler(wrong_h);
    }
    
    ~TestObject() 
    {
        dp->unregisterHandler(right_h);
        dp->unregisterHandler(wrong_h);
        
        delete right_h;
        delete wrong_h;
    }
    
    void right(const W::Object& data)
    {
        emit success();
    }
    
    void wrong(const W::Object& data)
    {
        
    }

    W::Handler* right_h;
    W::Handler* wrong_h;
    W::Dispatcher* dp;
    
signals:
    void success();
    
public slots:
    void launch()
    {
        W::Event ev(W::Address({u"client", u"123", u"some_hop", u"main", u"right"}), W::String(u"hello!"));
        dp->pass(ev);
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

        W::Dispatcher* root_dp = new W::Dispatcher();
        
        TestObject *test_object = new TestObject(W::Address({u"client", u"123", u"some_hop", u"main"}), root_dp);
        
        QObject::connect(test_object, SIGNAL(success()), &app, SLOT(quit()));
        
        QTimer *timer = new QTimer(&app);
        QObject::connect(timer, SIGNAL(timeout()), test_object, SLOT(launch()));
        timer->start(0);
        
        app.exec();
        
        delete test_object;
        delete root_dp;
    }
};

#endif //TESTDISPATCHER_H