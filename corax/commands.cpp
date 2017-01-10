#include "commands.h"

Commands::Commands(const W::Address& address, QObject* parent):
    M::Vocabulary(address, parent),
    commands(new Map())
{
}

Commands::~Commands()
{
    Map::iterator beg = commands->begin();
    Map::iterator end = commands->end();
    
    for (; beg != end; ++beg) {
        Command* cmd = beg->second;
        if (cmd->enabled) {
            removeHandler(cmd->handler);
        }
        delete cmd->handler;
        delete cmd;
    }
    
    delete commands;
}

void Commands::addCommand(const W::String& key, W::Handler* handler, const W::Vocabulary& args)
{
    Map::const_iterator itr = commands->find(key);
    if (itr != commands->end()) {
        throw 1;
    }
    Command* cmd = new Command{key, handler, args, false};
    commands->insert(std::make_pair(cmd->name, cmd));
}

void Commands::enableCommand(const W::String& key, bool value)
{
    Map::const_iterator itr = commands->find(key);
    if (itr == commands->end()) {
        throw 2;
    }
    
    Command* cmd = itr->second;
    if (cmd->enabled != value) {
        if (value) {
            enableCommand(cmd);
        } else {
            disableCommand(cmd);
        }
    }
}

void Commands::enableCommand(Commands::Command* cmd)
{
    addHandler(cmd->handler);
    cmd->enabled = true;
    
    W::Vocabulary* vc = new W::Vocabulary;
    vc->insert(u"address", cmd->handler->getAddress());
    vc->insert(u"argumanets", cmd->arguments);
    insert(cmd->name, vc);
}

void Commands::disableCommand(Commands::Command* cmd)
{
    removeHandler(cmd->handler);
    cmd->enabled = false;
    erase(cmd->name);
}
