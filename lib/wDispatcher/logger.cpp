#include "logger.h"

#include "iostream"

W::Logger::Logger():
    DefaultHandler()
{

}

W::Logger::~Logger()
{

}

bool W::Logger::call(const W::Event& ev) const
{
    std::cout << "Event went to default handler.\n";
    std::cout << "Destination: " << ev.getDestination().toString() << "\n";
    std::cout << "Data: " << ev.getData().toString();
    std::cout << std::endl;
    
    return false;
}
