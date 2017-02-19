#ifndef PARENTREPORTER_H
#define PARENTREPORTER_H

#include "defaulthandler.h"
#include "handler.h"
#include <map>

namespace W {
    
    class ParentReporter : public DefaultHandler
    {
    public:
        ParentReporter();
        ~ParentReporter();
        
        bool call(const W::Event& ev) const;
        void registerParent(const W::Address& address, W::Handler* handler);
        
    private:
        typedef std::map<W::Address, W::Handler*> Hmap;
        Hmap handlers;
    };
}

#endif // PARENTREPORTER_H
