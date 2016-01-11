#ifndef TESTSOCKET_H
#define TESTSOCKET_H

#include <cxxtest/TestSuite.h>

#include <QtCore/QCoreApplication>

class TestSocket : public CxxTest::TestSuite
{
    
public:
    void testHandshake()
    {
        TS_ASSERT_EQUALS(4, 4);
    }
};

#endif //TESTSOCKET_H