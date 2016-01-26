#include "handler.h"

W::Handler::Handler(const W::Address& p_rel_addr): 
    address(p_rel_addr)
{

}

W::Handler::~Handler()
{

}

const W::Address& W::Handler::getAddress() const
{
    return address;
}