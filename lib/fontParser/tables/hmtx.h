#ifndef HMTX_H
#define HMTX_H

#include "table.h"
#include <vector>

class HMtx : public Table
{
public:
    HMtx(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length);
    ~HMtx();
    
    uint16_t numOfLongHorMetrics;
    
    struct HMetric {
        HMetric();
        
        uint16_t advanceWidth;
        int16_t leftSideBearing;
    };
    
    void read(const std::string & path) override;
    HMetric getMetric(uint16_t cid) const;
    
private:
    std::vector<HMetric>* longHorMetric;
};

#endif // HMTX_H
