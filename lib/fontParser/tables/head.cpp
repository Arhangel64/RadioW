#include "head.h"
#include <arpa/inet.h>

HEad::HEad(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length):
    Table(p_tag, p_checkSum, p_offset, p_length),
    fontRevisionMajor(0),
    fontRevisionMinor(0),
    flags(0),
    unitsPerEm(0),
    xMin(0),
    yMin(0),
    xMax(0),
    yMax(0),
    macStyle(0),
    lowestRecPPEM(0),
    fontDirectionHint(0),
    indexToLocFormat(0)
{
}

HEad::~HEad()
{
}

void HEad::read(const std::string& path)
{
    char * buffer;
    buffer = new char[2];
    
    std::ifstream file(path, std::ios::in | std::ios::binary);
    file.seekg(offset);
    
    file.read(buffer, 2);   //version is not interesting, it is always 16.16 fixed point number equals to "1.0";
    file.read(buffer, 2);   //version is not interesting, it is always 16.16 fixed point number equals to "1.0";
    
    file.read(buffer, 2);
    fontRevisionMajor = ntohs(*((uint16_t*) buffer));
    
    file.read(buffer, 2);
    fontRevisionMinor = ntohs(*((uint16_t*) buffer));
    
    delete[] buffer;
    buffer = new char[4];
    file.read(buffer, 4);   //checkSumAdjustment - it's something fishy, no idea what to use it for;
    file.read(buffer, 4);   //magicNumber, always set to 0x5f0f3cf5;
    delete[] buffer;
    buffer = new char[2];
    
    file.read(buffer, 2);
    flags = ntohs(*((uint16_t*) buffer));
    
    file.read(buffer, 2);
    unitsPerEm = ntohs(*((uint16_t*) buffer));

    file.read(buffer, 2);   //creation date is a signed int64 
    file.read(buffer, 2);
    file.read(buffer, 2);
    file.read(buffer, 2);
    
    file.read(buffer, 2);   //last modification date is a signed int64 
    file.read(buffer, 2);
    file.read(buffer, 2);
    file.read(buffer, 2);
    
    file.read(buffer, 2);
    xMin = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    yMin = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    xMax = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    yMax = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    macStyle = ntohs(*((uint16_t*) buffer));
    
    file.read(buffer, 2);
    lowestRecPPEM = ntohs(*((uint16_t*) buffer));
    
    file.read(buffer, 2);
    fontDirectionHint = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    indexToLocFormat = ntohs(*((int16_t*) buffer));
    
    //and there is stil uint16 glyph data format, but its always 0;
    
    file.close();
    delete[] buffer;
}
