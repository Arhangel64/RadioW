#ifndef TESTDISPATCHER_H
#define TESTDISPATCHER_H

#include <cxxtest/TestSuite.h>

#include <QtCore/QCoreApplication>

#include <wDispatcher/dispatcher.h>
#include <wDispatcher/handler.h>

class TestObject: public QObject
{
public:
    TestObject(W::String hop):
        QObject(),
        dp(W::Address({hop}), this),
        right_h(0),
        wrong_h(0)
    {
        right_h = W::Handler::create(W::Address({U"right"}), this, &TestObject::right);
    }
    
    void right(const W::Event& ev)
    {
        
    }
    
    void wrong(const W::Event& ev)
    {
        
    }
    
    W::Dispatcher dp;
    W::Handler* right_h;
    W::Handler* wrong_h;
    
};

class TestDispatcher : public CxxTest::TestSuite
{
    
public:
    void testHandshake()
    {
        char a1[] = "nothing";
        char* argv[] = {a1};
        int   argc   = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
        //QCoreApplication app (argc, argv);
        
        //QObject::connect(srv, SIGNAL(success()), app, SLOT(quit()));
      
        //app.exec();
    }
};

#endif //TESTDISPATCHER_H