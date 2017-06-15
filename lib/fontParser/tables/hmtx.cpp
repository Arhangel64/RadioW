#include "hmtx.h"
#include <arpa/inet.h>

HMtx::HMtx(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length):
    Table(p_tag, p_checkSum, p_offset, p_length),
    numOfLongHorMetrics(0),
    longHorMetric(0)
{
}

HMtx::~HMtx()
{
    delete longHorMetric;
}

void HMtx::read(const std::string& path)
{
    if (numOfLongHorMetrics == 0) {
        throw 1;
    }
    
    std::ifstream file(path, std::ios::in | std::ios::binary);
    file.seekg(offset);
    
    char * buffer;
    buffer = new char[2];
    
    longHorMetric = new std::vector<HMetric>(numOfLongHorMetrics);
    
    for (int i = 0; i < numOfLongHorMetrics; ++i) {
        HMetric& met = longHorMetric->at(i);
        
        file.read(buffer, 2);
        uint16_t aw = ntohs(*((uint16_t*) buffer));
        
        file.read(buffer, 2);
        int16_t lsb = ntohs(*((int16_t*) buffer));
        
        met.advanceWidth = aw;
        met.leftSideBearing = lsb;
    }
    file.close();
    delete[] buffer;
}

HMtx::HMetric::HMetric():
    advanceWidth(0),
    leftSideBearing(0)
{
}

HMtx::HMetric HMtx::getMetric(uint16_t cid) const
{
    if (cid >= longHorMetric->size()) {
        cid = longHorMetric->size() - 1;
    }
    
    return longHorMetric->at(cid);
}
