#include "logger.h"

#include "iostream"

W::Logger::Logger():
    DefaultHandler()
{

}

W::Logger::~Logger()
{

}

void W::Logger::call(const W::Event& ev) const
{
    std::cout << "Event went to default handler.";
    std::cout << "Destination: " << ev.getDestination().toString();
    std::cout << "Data: " << ev.getData().toString();
    std::cout << std::endl;
}
