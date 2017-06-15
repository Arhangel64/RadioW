#ifndef HHEA_H
#define HHEA_H

#include "table.h"

class HHea : public Table
{
public:
    HHea(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length);
    ~HHea();
    
    void read(const std::string & path) override;
    
    int16_t ascent;
    int16_t descent;
    int16_t lineGap;
    uint16_t advanceWidthMax;
    int16_t minLeftSideBearing;
    int16_t minRightSideBearing;
    int16_t xMaxExtent;
    int16_t caretSlopeRise;
    int16_t caretSlopeRun;
    int16_t caretOffset;
    uint16_t numOfLongHorMetrics;
};

#endif // HHEA_H
