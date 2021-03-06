#include "catalogue.h"

M::Catalogue::Catalogue(const W::Address p_address, QObject* parent):
    ICatalogue(p_address, parent),
    data()
{
}

M::Catalogue::~Catalogue()
{
}

uint64_t M::Catalogue::addElement(const W::Vocabulary& record)
{
    uint64_t id = M::ICatalogue::addElement(record);
    
    data.insert(std::make_pair(id, static_cast<W::Vocabulary*>(record.copy())));
    return id;
}

void M::Catalogue::removeElement(uint64_t id)
{
    M::ICatalogue::removeElement(id);
    
    Data::const_iterator itr = data.find(id);
    delete itr->second;
    data.erase(itr);
}

void M::Catalogue::clear()
{
    M::ICatalogue::clear();
    
    data.clear();
}


W::Vocabulary * M::Catalogue::getElement(uint64_t id)
{
    return static_cast<W::Vocabulary*>(data.at(id)->copy());
}
std::set<uint64_t> M::Catalogue::getAll() const
{
    std::set<uint64_t> res;
    
    Data::const_iterator itr = data.begin();
    Data::const_iterator end = data.end();
    
    for (; itr != end; ++itr) {
        res.insert(itr->first);
    }
    
    return res;
}

void M::Catalogue::modifyElement(uint64_t id, const W::Vocabulary& newValue)
{
    Data::iterator itr = data.find(id);
    delete itr->second;
    itr->second = static_cast<W::Vocabulary*>(newValue.copy());
}

uint64_t M::Catalogue::size() const
{
    return data.size();
}
