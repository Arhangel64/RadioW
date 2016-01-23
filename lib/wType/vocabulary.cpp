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

W::Object::size_type W::Vocabulary::size() const
{
    return data->size();
}

W::Object::objectType W::Vocabulary::getType() const
{
    return Vocabulary::type;
}

void W::Vocabulary::serialize(W::ByteArray& out) const
{
    pushSize(out);
    
    Map::const_iterator itr = data->begin();
    Map::const_iterator end = data->end();
    for (; itr != end; ++itr)
    {
        itr->first.serialize(out);
        out.push(itr->second->getType());
        itr->second->serialize(out);
    }
}

void W::Vocabulary::deserialize(W::ByteArray& in)
{
    data->clear();
    
    size_type length = popSize(in);
    
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
