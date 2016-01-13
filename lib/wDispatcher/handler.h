#ifndef HANDLER_H
#define HANDLER_H

#include "node.h"

namespace W 
{
    class Handler: public Node
    {
        Q_OBJECT
    public:
        Handler(const Address& p_rel_addr, QObject* parent = 0);
        ~Handler();
    };
}

#endif // HANDLER_H
