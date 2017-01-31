#include "commands.h"

Commands::Commands(const W::Address& address, QObject* parent):
    C::Vocabulary(address, parent)
{
}

Commands::~Commands()
{
}
