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
#include <wType/vector.h>

class TestTypes : public CxxTest::TestSuite
{
    
public:
    void testStringSize() 
    {
        W::String str(u"hey!");
        TS_ASSERT_EQUALS(str.size(), 4);
    }
    
    void testStringToString()
    {
        W::String str(u"hello world!");
        TS_ASSERT_EQUALS(str.toString(), "hello world!");
    }
    
    void testStringToString2()
    {
        W::String str(u"Сраные стандарты стингов!");
        TS_ASSERT_EQUALS(str.toString(), "Сраные стандарты стингов!");
    }
    void testStringCopying()
    {
        W::String str(u"string");
        W::String str2 = str; 
    }
    
    void testStringSerialization()
    {
        W::String str(u"serialization");
        W::ByteArray bytes;
        bytes << str;
        TS_ASSERT_EQUALS(bytes.size(), 13*2 + 1*4 + 1);
        
        W::Object *obj = W::Object::fromByteArray(bytes);
        W::String *str2 = static_cast<W::String*>(obj);
        
        TS_ASSERT_EQUALS(bytes.size(), 0);
        TS_ASSERT_EQUALS(str.toString(), "serialization");
        TS_ASSERT_EQUALS(str2->toString(), "serialization");
        
        delete obj;
    }
    void testStringSerialization2()
    {
        W::String str(u"разве сложно сразу сделать все нормально?!");
        W::ByteArray bytes;
        bytes << str;
        TS_ASSERT_EQUALS(bytes.size(), 42*2 + 1*4 + 1);
        
        W::Object *obj = W::Object::fromByteArray(bytes);
        W::String *str2 = static_cast<W::String*>(obj);
        
        TS_ASSERT_EQUALS(bytes.size(), 0);
        TS_ASSERT_EQUALS(str.toString(), "разве сложно сразу сделать все нормально?!");
        TS_ASSERT_EQUALS(str2->toString(), "разве сложно сразу сделать все нормально?!");
        TS_ASSERT_EQUALS(str, *str2);
        
        delete obj;
    }
    void testUint64Serialization()
    {
        W::Uint64 a(895458745634);
        W::ByteArray bytes;
        
        bytes << a;
        
        TS_ASSERT_EQUALS(bytes.size(), 1 + 8);
        
        W::Object *obj = W::Object::fromByteArray(bytes);
        W::Uint64 *b = static_cast<W::Uint64*>(obj);
        
        TS_ASSERT_EQUALS(a, *b);
        delete obj;
    }
    void testVCSerialization()
    {
        W::String key1(u"foo");
        W::String val1(u"bar");
        
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
    void testVectorSerialization() 
    {
        W::String str1(u"foo");
        W::String str2(u"bar");
        
        W::Vector vec;
        W::ByteArray bytes;
        
        vec.push(str1);
        vec.push(str2);
        vec.push(str2);
        vec.push(str2);
        vec.push(str1);
        
        TS_ASSERT_EQUALS(vec.size(), 5);
        TS_TRACE(vec.toString());
        
        bytes << vec;
        W::Object* deserialized = W::Object::fromByteArray(bytes);
        W::Vector* dvec = static_cast<W::Vector*>(deserialized);
        
        TS_ASSERT_EQUALS(vec.size(), dvec->size());
        W::String str22 = static_cast<const W::String&>(dvec->at(3));
        
        TS_ASSERT_EQUALS(str2, str22);
        
        delete deserialized;
    }
    void testAddressOperators()
    {
        W::Address a1({u"hey"});
        W::Address a2({u"hey", u"you"});
        W::Address a3({u"hey1", u"you"});
        W::Address a4({u"hey", u"you1"});
        
        TS_ASSERT_EQUALS(a1, a1);
        TS_ASSERT_DIFFERS(a1, a2);
        TS_ASSERT_LESS_THAN(a1, a2);
        TS_ASSERT_LESS_THAN(a2, a3);
        TS_ASSERT_LESS_THAN(a2, a4);
        TS_ASSERT_LESS_THAN(a4, a3);
    }
    void testAddressFunctions()
    {
        W::Address a1({u"1st", u"2nd", u"3rd", u"4th"});
        W::Address a2 = a1 >> 1;
        W::Address a3 = a1 << 1;
        
        W::Address ae;
        W::Address a4({u"1st"});
        W::Address a5({u"1st", u"2nd"});
        
        W::Address a6({u"1st", u"3rd"});
        
        W::Address a7({u"3rd", u"4th"});
        W::Address a8({u"4th"});
        
        W::Address a2c({u"1st", u"2nd", u"3rd"});
        W::Address a3c({u"2nd", u"3rd", u"4th"});
        
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
        W::Address addr({u"hello", u"world"});
        
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
        W::Address dest({u"to", u"somebody"});
        W::Uint64 id(5);
        W::Vocabulary dat;
        W::String val(u"some value");
        W::Uint64 val2(7887198479813);
        dat.insert(u"key1", val);
        dat.insert(u"key2", val2);
        
        W::Event ev(dest, dat);
        ev.setSenderId(id);
        
        W::ByteArray bytes;
        
        bytes << ev;
        
        W::Object *obj = W::Object::fromByteArray(bytes);
        W::Event *evd = static_cast<W::Event*>(obj);
        
        TS_ASSERT_EQUALS(evd->isSystem(), false);
        TS_ASSERT_EQUALS(evd->getDestination(), dest);
        const W::Vocabulary vcd = static_cast<const W::Vocabulary&>(evd->getData());
        TS_ASSERT_EQUALS(static_cast<const W::String&>(vcd.at(u"key1")), val);
        TS_ASSERT_EQUALS(static_cast<const W::Uint64&>(vcd.at(u"key2")), val2);
        TS_ASSERT_EQUALS(evd->getSenderId(), id);
        
        delete obj;
    }
};


#endif
