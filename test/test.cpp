/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_TestTypes_init = false;
#include "testTypes.h"

static TestTypes suite_TestTypes;

static CxxTest::List Tests_TestTypes = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestTypes( "testTypes.h", 9, "TestTypes", suite_TestTypes, Tests_TestTypes );

static class TestDescription_suite_TestTypes_testString : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestTypes_testString() : CxxTest::RealTestDescription( Tests_TestTypes, suiteDescription_TestTypes, 13, "testString" ) {}
 void runTest() { suite_TestTypes.testString(); }
} testDescription_suite_TestTypes_testString;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
