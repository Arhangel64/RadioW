#ifndef NAME_H
#define NAME_H

#include <map>
#include <string>

#include "table.h"

class Name : public Table
{
public:
    Name(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length);
    ~Name();
    
    void read(const std::string & path) override;
    std::string getRecord(uint16_t id) const;
    std::string getRecord(const std::string& name) const;
    
private:
    std::map<uint16_t, std::string> names;
    
    static const std::map<std::string, uint16_t> nameIds;
};

struct NameRecord {
    NameRecord(uint16_t pid, uint16_t eid, uint16_t lid, uint16_t nid, uint16_t p_l, uint16_t p_o);
    uint16_t platformId;
    uint16_t encodingId;
    uint16_t languageId;
    uint16_t nameId;
    uint16_t length;
    uint16_t offset;
};

#endif // NAME_H
