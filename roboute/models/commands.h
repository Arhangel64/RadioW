#ifndef ROBOUTE_COMMANDS_H
#define ROBOUTE_COMMANDS_H

#include <wController/vocabulary.h>

class Commands : public C::Vocabulary
{
public:
    Commands(const W::Address& address, QObject* parent = 0);
    ~Commands();
};

#endif // ROBOUTE_COMMANDS_H
