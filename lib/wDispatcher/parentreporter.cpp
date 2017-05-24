#include "parentreporter.h"

W::ParentReporter::ParentReporter():
    W::DefaultHandler(),
    handlers()
{
}

W::ParentReporter::~ParentReporter()
{
}

bool W::ParentReporter::call(const W::Event& ev) const
{
    const W::Address& addr = ev.getDestination();
    std::map<int, W::Handler*> result;
    
    Hmap::const_iterator itr = handlers.begin();
    Hmap::const_iterator end = handlers.end();
    
    for (; itr != end; ++itr) {                 //need to find the closest parent to the event destination
        if (addr.begins(itr->first)) {          //the closest parent has the longest address of those whose destinatiion begins with
            result.insert(std::make_pair(itr->first.size(), itr->second));
        }
    }
    if (result.size() > 0) {
        std::map<int, W::Handler*>::const_iterator itr = result.end();
        --itr;
        itr->second->pass(ev);                  //need to report only to the closest parent
        return true;
    } else {
        return false;
    }
}

void W::ParentReporter::registerParent(const W::Address& address, W::Handler* handler)
{
    Hmap::const_iterator itr = handlers.find(address);
    if (itr != handlers.end()) {
        throw 1;
    } else {
        handlers.insert(std::make_pair(address, handler));
    }
}
