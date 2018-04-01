#include "vocabulary.h"

W::Vocabulary::Vocabulary():
    Object(),
    data(new Map())
{

}

W::Vocabulary::Vocabulary(const W::Vocabulary& original):
    Object(),
    data(new Map())
{
    Map::const_iterator itr = original.data->begin();
    Map::const_iterator end = original.data->end();
    
    for (; itr != end; ++itr)
    {
        (*data)[itr->first] = itr->second->copy();
    }
}


W::Vocabulary::~Vocabulary()
{
    clear();
    delete data;
}

W::Vocabulary& W::Vocabulary::operator=(const W::Vocabulary& original)
{
    if (&original != this)
    {
        clear();
        Map::const_iterator itr = original.data->begin();
        Map::const_iterator end = original.data->end();
    
        for (; itr != end; ++itr)
        {
            (*data)[itr->first] = itr->second->copy();
        }
    }
    return *this;
}

void W::Vocabulary::clear()
{
    Map::iterator itr = data->begin();
    Map::iterator end = data->end();
    
    for (; itr != end; ++itr)
    {
        delete itr->second;
    }
    
    data->clear();
}

W::Object::StdStr W::Vocabulary::toString() const
{
    StdStr result;
    
    Map::const_iterator itr;
    Map::const_iterator beg = data->begin();
    Map::const_iterator end = data->end();
    
    result += "{";
    for (itr = beg; itr != end; ++itr)
    {
        if (itr != beg)
        {
            result += ", ";
        }
        result += itr->first.toString();
        result += ": ";
        result += itr->second->toString();
    }
    result += "}";
    
    return result;
}

W::Object* W::Vocabulary::copy() const
{
    return new Vocabulary(*this);
}

W::Object::size_type W::Vocabulary::length() const
{
    return data->size();
}

W::Object::size_type W::Vocabulary::size() const
{
    size_type size = 4;
    
    Map::const_iterator itr = data->begin();
    Map::const_iterator end = data->end();
    
    for (; itr != end; ++itr)
    {
        size += itr->first.size();
        size += itr->second->size() + 1;
    }
    
    return size;
}


W::Object::objectType W::Vocabulary::getType() const
{
    return Vocabulary::type;
}

void W::Vocabulary::serialize(W::ByteArray& out) const
{
    out.push32(length());
    
    Map::const_iterator itr = data->begin();
    Map::const_iterator end = data->end();
    for (; itr != end; ++itr)
    {
        itr->first.serialize(out);
        out.push8(itr->second->getType());
        itr->second->serialize(out);
    }
}

void W::Vocabulary::deserialize(W::ByteArray& in)
{
    data->clear();
    
    size_type length = in.pop32();
    
    for (size_type i = 0; i != length; ++i)
    {
        String key;
        key.deserialize(in);
        Object* value = Object::fromByteArray(in);
        
        (*data)[key] = value;
    }
}

void W::Vocabulary::insert(const W::String::u16string& key, const W::Object& value)
{
    String strKey(key);
    insert(strKey, value);
}

void W::Vocabulary::insert(const W::String& key, const W::Object& value)
{
    Map::const_iterator itr = data->find(key);
    
    if (itr != data->end()) 
    {
        delete itr->second;
    }
    (*data)[key] = value.copy();
}

void W::Vocabulary::insert(const String::u16string& key, W::Object* value)
{
    String strKey(key);
    insert(strKey, value);
}

void W::Vocabulary::insert(const W::String& key, W::Object* value)
{
    Map::const_iterator itr = data->find(key);
    
    if (itr != data->end()) 
    {
        delete itr->second;
    }
    (*data)[key] = value;
}


const W::Object& W::Vocabulary::at(const String::u16string& key) const
{
    String strKey(key);
    return at(strKey);
}

const W::Object& W::Vocabulary::at(const W::String& key) const
{
    Map::const_iterator itr = data->find(key);
    
    if (itr == data->end())
    {
        throw NoElement(key);
    }
    
    return *(itr->second);
}

bool W::Vocabulary::has(const String::u16string& key) const
{
    String strKey(key);
    return has(strKey);
}

bool W::Vocabulary::has(const W::String& key) const
{
    Map::const_iterator itr = data->find(key);
    
    return itr != data->end();
}

void W::Vocabulary::erase(const String::u16string& key)
{
    String strKey(key);
    erase(strKey);
}

void W::Vocabulary::erase(const W::String& key)
{
    Map::const_iterator itr = data->find(key);
    
    if (itr == data->end())
    {
        throw NoElement(key);
    }
    
    data->erase(itr);
}

W::Vector W::Vocabulary::keys() const
{
    Vector keys;
    Map::iterator itr = data->begin();
    Map::iterator end = data->end();
    
    for (; itr != end; ++itr)
    {
        keys.push(itr->first);
    }
    
    return keys;
}


W::Vocabulary::NoElement::NoElement(const W::String& p_key):
    Exception(),
    key(p_key)
{
    
}


std::string W::Vocabulary::NoElement::getMessage() const
{
    std::string msg("No element has been found by key: ");
    msg += key.toString();
    
    return msg;
}

bool W::Vocabulary::operator==(const W::Object& other) const
{
    if (sameType(other)) {
        return operator==(static_cast<const W::Vocabulary&>(other));
    } else {
        return false;
    }
}

bool W::Vocabulary::operator==(const W::Vocabulary& other) const
{
    bool equal = data->size() == other.data->size();
    Map::const_iterator mItr = data->begin();
    Map::const_iterator oItr = other.data->begin();
    
    while (equal && mItr != data->end()) {
        equal = (mItr->first == oItr->first) && (mItr->second == oItr->second);
        ++mItr;
        ++oItr;
    }
    
    return equal;
}

W::Vocabulary * W::Vocabulary::extend(const W::Vocabulary& first, const W::Vocabulary& second)
{
    W::Vocabulary* vc = static_cast<W::Vocabulary*>(first.copy());
    
    Map::const_iterator itr = second.data->begin();
    Map::const_iterator end = second.data->end();
    
    for (; itr != end; ++itr) {
        vc->data->operator[](itr->first) = itr->second;
    }
    
    return vc;
}
