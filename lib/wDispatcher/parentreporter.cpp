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
    Hmap::const_iterator itr = handlers.find(ev.getDestination());
    if (itr == handlers.end()) {
        return false;
    } else {
        itr->second->pass(ev);
        return true;
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
