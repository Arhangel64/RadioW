#ifndef TESETYPES_H
#define TESTTYPES_H

#include <cxxtest/TestSuite.h>
#include <string>

#include <wType/object.h>
#include <wType/bytearray.h>
#include <wType/string.h>
#include <wType/vocabulary.h>
#include <wType/uint64.h>
#include <wType/address.h>

class TestTypes : public CxxTest::TestSuite
{
    
public:
    void testStringSize() 
    {
        W::String str(U"hey!");
        TS_ASSERT_EQUALS(str.size(), 4);
    }
    
    void testStringToString()
    {
        W::String str(U"hello world!");
        TS_ASSERT_EQUALS(str.toString(), "hello world!");
    }
    void testStringCopying()
    {
        W::String str(U"string");
        W::String str2 = str; 
    }
    
    void testStringSerialization()
    {
        W::String str(U"serialization");
        W::ByteArray bytes;
        bytes << str;
        TS_ASSERT_EQUALS(bytes.size(), (13 + 1 + 1)*4);
        
        W::Object *obj = W::Object::fromByteArray(bytes);
        W::String *str2 = static_cast<W::String*>(obj);
        
        TS_ASSERT_EQUALS(bytes.size(), 0);
        TS_ASSERT_EQUALS(str.toString(), "serialization");
        TS_ASSERT_EQUALS(str2->toString(), "serialization");
        
        delete obj;
    }
    void testUint64Serialization()
    {
        W::Uint64 a(895458745634);
        W::ByteArray bytes;
        
        bytes << a;
        
        TS_ASSERT_EQUALS(bytes.size(), 3 * 4);
        
        W::Object *obj = W::Object::fromByteArray(bytes);
        W::Uint64 *b = static_cast<W::Uint64*>(obj);
        
        TS_ASSERT_EQUALS(a, *b);
        delete obj;
    }
    void testVCSerialization()
    {
        W::String key1(U"foo");
        W::String val1(U"bar");
        
        W::Vocabulary vc;
        
        W::ByteArray bytes;
        
        vc.insert(key1, val1);
        TS_ASSERT_EQUALS(vc.size(), 1);
        
        TS_TRACE(vc.toString());
        
        bytes << vc;
        
        W::Object* deserialized = W::Object::fromByteArray(bytes);
        W::Vocabulary* dvc = static_cast<W::Vocabulary*>(deserialized);
        
        TS_ASSERT_EQUALS(vc.size(), dvc->size());
        W::String val2 = static_cast<const W::String&>(dvc->at(key1));
        TS_ASSERT_EQUALS(val2, val1);
        
        delete deserialized;
    }
    void testAddressOperators()
    {
        W::Address a1({U"hey"});
        W::Address a2({U"hey", U"you"});
        W::Address a3({U"hey1", U"you"});
        W::Address a4({U"hey", U"you1"});
        
        TS_ASSERT_EQUALS(a1, a1);
        TS_ASSERT_DIFFERS(a1, a2);
        TS_ASSERT_LESS_THAN(a1, a2);
        TS_ASSERT_LESS_THAN(a2, a3);
        TS_ASSERT_LESS_THAN(a2, a4);
        TS_ASSERT_LESS_THAN(a4, a3);
    }
    void testAddressSerialization()
    {
        W::Address addr({U"hello", U"world"});
        
        W::ByteArray bytes;
        bytes << addr;
        
        W::Object *obj = W::Object::fromByteArray(bytes);
        W::Address *addrd = static_cast<W::Address*>(obj);
        
        TS_ASSERT_EQUALS(addr, *addrd);
        TS_TRACE(addr.toString());
        TS_TRACE(addrd->toString());
        
        delete addrd;
    }
};


#endif