#include "hhea.h"
#include <arpa/inet.h>

Hhea::Hhea(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length):
    Table(p_tag, p_checkSum, p_offset, p_length),
    ascent(0),
    descent(0),
    lineGap(0),
    advanceWidthMax(0),
    minLeftSideBearing(0),
    minRightSideBearing(0),
    xMaxExtent(0),
    caretSlopeRise(0),
    caretSlopeRun(0),
    caretOffset(0),
    numOfLongHorMetrics(0)
{
}

Hhea::~Hhea()
{
}

void Hhea::read(const std::string& path)
{
    char * buffer;
    buffer = new char[2];
    
    std::ifstream file(path, std::ios::in | std::ios::binary);
    file.seekg(offset);
    
    file.read(buffer, 2);   //version is not interesting, it is always 16.16 fixed point number equals to "1.0";
    file.read(buffer, 2);   //version is not interesting, it is always 16.16 fixed point number equals to "1.0";
    
    file.read(buffer, 2);
    ascent = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    descent = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    lineGap = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    advanceWidthMax = ntohs(*((uint16_t*) buffer));
    
    file.read(buffer, 2);
    minLeftSideBearing = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    minRightSideBearing = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    xMaxExtent = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    caretSlopeRise = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    caretSlopeRun = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);
    caretOffset = ntohs(*((int16_t*) buffer));
    
    file.read(buffer, 2);   //reserved empty field, supposed to be 0;
    file.read(buffer, 2);   //reserved empty field, supposed to be 0;
    file.read(buffer, 2);   //reserved empty field, supposed to be 0;
    file.read(buffer, 2);   //reserved empty field, supposed to be 0;
    file.read(buffer, 2);   //metricDataFormat, it's supposed to be 0;
    
    file.read(buffer, 2);
    numOfLongHorMetrics = ntohs(*((uint16_t*) buffer));
    
    delete[] buffer;
    file.close();
}
