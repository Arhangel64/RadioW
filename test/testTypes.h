#ifndef TESTTYPES_H
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
#include <wType/event.h>

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
    void testAddressFunctions()
    {
        W::Address a1({U"1st", U"2nd", U"3rd", U"4th"});
        W::Address a2 = a1 >> 1;
        W::Address a3 = a1 << 1;
        
        W::Address ae;
        W::Address a4({U"1st"});
        W::Address a5({U"1st", U"2nd"});
        
        W::Address a6({U"1st", U"3rd"});
        
        W::Address a7({U"3rd", U"4th"});
        W::Address a8({U"4th"});
        
        W::Address a2c({U"1st", U"2nd", U"3rd"});
        W::Address a3c({U"2nd", U"3rd", U"4th"});
        
        TS_ASSERT_EQUALS(a2, a2c);
        TS_ASSERT_EQUALS(a3, a3c);
        
        TS_ASSERT(a4.begins(ae));
        TS_ASSERT(a4.ends(ae));
        
        TS_ASSERT(a1.begins(ae));
        TS_ASSERT(a1.ends(ae));
        
        TS_ASSERT(a1.begins(a4));
        TS_ASSERT(a1.begins(a5));
        TS_ASSERT(!a1.begins(a6));
        
        TS_ASSERT(a1.ends(a7));
        TS_ASSERT(a1.ends(a8));
        TS_ASSERT(!a1.ends(a6));
        
        TS_ASSERT(a1.begins(a2c));
        TS_ASSERT(a1.ends(a3c));
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
    void testEventSerialization()
    {
        W::Address dest({U"to", U"somebody"});
        W::Uint64 id(5);
        W::Vocabulary dat;
        W::String val(U"some value");
        W::Uint64 val2(7887198479813);
        dat.insert(U"key1", val);
        dat.insert(U"key2", val2);
        
        W::Event ev(dest, dat);
        ev.setSenderId(id);
        
        W::ByteArray bytes;
        
        bytes << ev;
        
        W::Object *obj = W::Object::fromByteArray(bytes);
        W::Event *evd = static_cast<W::Event*>(obj);
        
        TS_ASSERT_EQUALS(evd->isSystem(), false);
        TS_ASSERT_EQUALS(evd->getDestination(), dest);
        const W::Vocabulary vcd = static_cast<const W::Vocabulary&>(evd->getData());
        TS_ASSERT_EQUALS(static_cast<const W::String&>(vcd.at(U"key1")), val);
        TS_ASSERT_EQUALS(static_cast<const W::Uint64&>(vcd.at(U"key2")), val2);
        TS_ASSERT_EQUALS(evd->getSenderId(), id);
        
        delete obj;
    }
};


#endif