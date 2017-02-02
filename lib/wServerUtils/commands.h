#ifndef SERVERUTILS_COMMANDS_H
#define SERVERUTILS_COMMANDS_H

#include <map>

#include <wType/address.h>
#include <wType/vocabulary.h>
#include <wType/event.h>
#include <wType/string.h>

#include <wModel/vocabulary.h>

namespace U {

    class Commands : public M::Vocabulary
    {
        struct Command;
        typedef std::map<W::String, Command*> Map;
    public:
        Commands(const W::Address& address, QObject* parent = 0);
        ~Commands();
        
        void addCommand(const W::String& key, W::Handler* handler, const W::Vocabulary& args);
        void enableCommand(const W::String& key, bool value);
        
    private:
        void enableCommand(Command* cmd);
        void disableCommand(Command* cmd);
        
        Map* commands;
    
        struct Command {
            W::String name;
            W::Handler* handler;
            W::Vocabulary arguments;
            bool enabled;
        };
    };

}

#endif // SERVERUTILS_COMMANDS_H
