#ifndef LOGGER_H
#define LOGGER_H

#include "defaulthandler.h"

#include <wType/event.h>

namespace W 
{
    class Logger:
        public DefaultHandler
    {
    public:
        Logger();
        ~Logger();
        
        bool call(const W::Event& ev) const;
    };
}

#endif // LOGGER_H
