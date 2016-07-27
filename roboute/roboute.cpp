#include "roboute.h"

#include <iostream>

using std::cout;
using std::endl;

Roboute* Roboute::roboute = 0;

Roboute::Roboute(QObject *parent):
    QObject(parent),
    logger(new W::Logger()),
    dispatcher(new W::Dispatcher())
{
    if (roboute != 0) 
    {
        throw SingletonError();
    }
    Roboute::roboute = this;
    
    dispatcher->registerDefaultHandler(logger);
}

Roboute::~Roboute()
{
    dispatcher->unregisterDefaultHandler(logger);
    
    delete logger;
    delete dispatcher;
    
    Roboute::roboute = 0;
}

void Roboute::start()
{
    debug("Starting roboute...");
    debug("Roboute is ready");
}

void Roboute::stop()
{
    debug("Stopping roboute...");
}

void Roboute::debug(std::string str)
{
    cout << str << endl;
    QString dbg = str.c_str();
    dbg.append("\n");
    emit debugMessage(dbg);
}
