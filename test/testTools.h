#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <cxxtest/TestSuite.h>

#include <tools/file.h>
#include <wType/string.h>
#include <taglib/fileref.h>
#include <iostream>

class TestUtils : public CxxTest::TestSuite
{
public:
    void testFile() {
        TagLib::FileRef ref("/home/betrayer/Music/Disturbed/Indestructible/Façade.mp3");
        TS_ASSERT_EQUALS(ref.tag()->title().to8Bit(true), "Façade");
        
        W::String wPath(u"/home/betrayer/Music/Disturbed/Indestructible/Façade.mp3");
        W::String wTitle(u"Façade");
        
        TagLib::FileRef ref2(wPath.toString().c_str());
        TS_ASSERT_EQUALS(W::String(ref.tag()->title().to8Bit(true)), wTitle);
    }
};

#endif //TESTUTILS_H
