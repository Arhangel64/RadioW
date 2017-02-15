#ifndef DEFAULTHANDLER_H
#define DEFAULTHANDLER_H

#include <wType/event.h>

namespace W 
{
    class DefaultHandler
    {
    public:
        DefaultHandler();
        virtual ~DefaultHandler();
        
        virtual bool call(const W::Event& ev) const = 0;
    };
}

#endif // DEFAULTHANDLER_H
