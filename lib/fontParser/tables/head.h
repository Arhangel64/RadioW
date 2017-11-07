#ifndef HEAD_H
#define HEAD_H

#include "table.h"

class Head : public Table
{
public:
    Head(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length);
    ~Head();
    
    void read(const std::string & path) override;
    
    uint16_t fontRevisionMajor;
    uint16_t fontRevisionMinor;
    uint16_t flags;
    uint16_t unitsPerEm;
    int16_t xMin;
    int16_t yMin;
    int16_t xMax;
    int16_t yMax;
    uint16_t macStyle;
    uint16_t lowestRecPPEM;
    int16_t fontDirectionHint;
    int16_t indexToLocFormat;
};

#endif // HEAD_H
