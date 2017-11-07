#ifndef CMAP_H
#define CMAP_H

#include "table.h"
#include <list>
#include <map>
#include <vector>

struct Enc {
    Enc(uint16_t pid, uint16_t eid, uint32_t off): platformId(pid), encodingId(eid), offset(off) {}
    
    uint16_t platformId;
    uint16_t encodingId;
    uint32_t offset;
};

class MappingTable {
protected:
    MappingTable(uint16_t p_f);
    
    uint16_t format;
    
public:
    static MappingTable* fromIfStream(std::ifstream& file);
    virtual ~MappingTable();
    virtual uint32_t getCID(uint32_t charCode) const = 0;
};

class Format4 : public MappingTable {
public:
    Format4(char* data, uint16_t length);
    ~Format4();
    
    uint32_t getCID(uint32_t charCode) const override;
    
private:
    struct SegParams {
        uint16_t endCode;
        uint16_t startCode;
        int16_t idDelta;
        uint16_t idRangeOffset;
    };
    
    std::map<uint16_t, uint16_t> charCodesEndCode;
    std::vector<SegParams>* segments;
    std::vector<uint16_t>* glyphIndexArray;
};

class Cmap : public Table
{
public:
    Cmap(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length);
    ~Cmap();
    
    void read(const std::string & path) override;
    uint32_t getCID(uint32_t charCode) const;
    
    uint16_t version;
    uint16_t numberOfTables;
    
private:
    bool initialized;
    MappingTable* mt;
};

#endif // CMAP_H
