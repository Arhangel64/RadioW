#ifndef NODE_H
#define NODE_H

#include <QObject>

#include <wType/address.h>
#include <wType/event.h>

namespace W
{
    class Node: public QObject
    {
        Q_OBJECT
        
    public:
        Node(const W::Address& p_rel_addr, QObject* parent = 0);
        virtual ~Node();
        
        virtual void pass(const W::Event& ev) const = 0;
        
        virtual void setRelativeAddress(const W::Address& addr);
        W::Address getRelativeAddress() const;
        
    protected:
        W::Address rel_address;
    };
}
#endif // NODE_H
