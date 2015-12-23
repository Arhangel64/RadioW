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
#include <wType/boolean.h>

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
    void testBooleanSerialization()
    {
        W::Boolean a(true);
        W::Boolean b(false);
        W::Boolean c;
        W::Boolean d;
        c = false;
        d = true;
        
        TS_ASSERT_EQUALS(a, true);
        TS_ASSERT_EQUALS(b, false);
        TS_ASSERT_EQUALS(c, false);
        TS_ASSERT_EQUALS(d, true);
        
        W::ByteArray bytes;
        bytes << a << b << c << d;
        
        W::Object *a_o = W::Object::fromByteArray(bytes);
        W::Object *b_o = W::Object::fromByteArray(bytes);
        W::Object *c_o = W::Object::fromByteArray(bytes);
        W::Object *d_o = W::Object::fromByteArray(bytes);
        
        W::Boolean *ad = static_cast<W::Boolean*>(a_o);
        W::Boolean *bd = static_cast<W::Boolean*>(b_o);
        W::Boolean *cd = static_cast<W::Boolean*>(c_o);
        W::Boolean *dd = static_cast<W::Boolean*>(d_o);
        
        TS_ASSERT_EQUALS(*ad, a);
        TS_ASSERT_EQUALS(*bd, b);
        TS_ASSERT_EQUALS(*cd, c);
        TS_ASSERT_EQUALS(*dd, d);
        
        TS_TRACE(ad->toString());
        TS_TRACE(bd->toString());
        
        delete ad;
        delete bd;
        delete cd;
        delete dd;
    }
};


#endif