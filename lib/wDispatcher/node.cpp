#include "node.h"

W::Node::Node(const W::Address& p_rel_addr, QObject* parent): 
    QObject(parent),
    rel_address(p_rel_addr)
{

}

W::Node::~Node()
{

}

void W::Node::setRelativeAddress(const W::Address& addr)
{
    rel_address = addr;
}

W::Address W::Node::getRelativeAddress() const
{
    return rel_address;
}
