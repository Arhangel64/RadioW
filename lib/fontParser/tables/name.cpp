#include "name.h"
#include <arpa/inet.h>
#include <list>
#include <set>
#include <codecvt>
#include <locale>

const std::map<std::string, uint16_t> Name::nameIds({
    {   "copyright",            0       },
    {   "fontFamily",           1       },
    {   "fontSubfamily",        2       },
    {   "uniqueSubfamilyId",    3       },
    {   "fullFontName",         4       },
    {   "nameTableVersion",     5       },
    {   "postScriptName",       6       },
    {   "trademarkNotice",      7       },
    {   "manufacturerName",     8       },
    {   "designerName",         9       },
    {   "description",          10      },
    {   "vendorURL",            11      },
    {   "designerURL",          12      },
    {   "licenseDescription",   13      },
    {   "licenseURL",           14      },
    
    {   "preferredFamily",      16      },
    {   "preferredSubfamily",   17      },
    {   "compatibleFull",       18      },
    {   "sampleText",           19      },
    {   "postScriptCID",        20      }
});

Name::Name(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length):
    Table(p_tag, p_checkSum, p_offset, p_length),
    names()
{
}

Name::~Name()
{
}

void Name::read(const std::string& path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    file.seekg(offset);
    
    char * buffer;
    buffer = new char[2];
    
    file.read(buffer, 2);       //format. it is always 0 or 1 for stupid microsoft langTags, but I don't cate, gonna use offset;
    file.read(buffer, 2);
    uint16_t count = ntohs(*((uint16_t*) buffer));
    
    file.read(buffer, 2);
    uint32_t storageOffset = offset + ntohs(*((uint16_t*) buffer));

    std::list<NameRecord> list;
    std::set<uint16_t> ids;
    
    for (int i = 0; i < count; ++i) {
        file.read(buffer, 2);
        uint16_t pid = ntohs(*((uint16_t*) buffer));
        file.read(buffer, 2);
        uint16_t eid = ntohs(*((uint16_t*) buffer));
        file.read(buffer, 2);
        uint16_t lid = ntohs(*((uint16_t*) buffer));
        file.read(buffer, 2);
        uint16_t nid = ntohs(*((uint16_t*) buffer));
        file.read(buffer, 2);
        uint16_t length = ntohs(*((uint16_t*) buffer));
        file.read(buffer, 2);
        uint16_t nameOffset = ntohs(*((uint16_t*) buffer));
        
        //std::cout << "Found pid " << pid << ", eid " << eid << ", nid " << nid << std::endl;
        
        if (ids.find(nid) == ids.end()) {
            if ((pid == 0 && (eid == 3 || eid == 4)) || (pid == 3 && eid == 1)) {         //screw microsoft, screw apple;
                list.emplace_back(pid, eid, lid, nid, length, nameOffset);
                ids.insert(nid);
            }
        }
        
    }
    std::list<NameRecord>::const_iterator itr;
    for (itr = list.begin(); itr != list.end(); ++itr) {
        const NameRecord& nr = *itr;
        file.seekg(storageOffset + nr.offset);
        
        if ((nr.platformId == 0 && (nr.encodingId == 3 || nr.encodingId == 4)) || (nr.platformId == 3 && nr.encodingId == 1)) {
            char16_t buf[nr.length / 2];
            for (int i = 0; i < nr.length / 2; ++i) {
                file.read(buffer, 2);
                buf[i] = ntohs(*((char16_t*) buffer));
            }
            std::u16string string(buf, nr.length / 2);
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
            names.insert(std::make_pair(nr.nameId, convert.to_bytes(string)));
        }
        
    }
    
    delete[] buffer;
    file.close();
}

std::string Name::getRecord(uint16_t id) const
{
    std::string res("");
    std::map<uint16_t, std::string>::const_iterator itr = names.find(id);
    if (itr != names.end()) {
        res = itr->second;
    }
    
    return res;
}

std::string Name::getRecord(const std::string& name) const
{
    std::map<std::string, uint16_t>::const_iterator itr = nameIds.find(name);
    if (itr == nameIds.end()) {
        return "";
    } else {
        return getRecord(itr->second);
    }
}


NameRecord::NameRecord(uint16_t pid, uint16_t eid, uint16_t lid, uint16_t nid, uint16_t p_l, uint16_t p_o):
    platformId(pid),
    encodingId(eid),
    languageId(lid),
    nameId(nid),
    length(p_l),
    offset(p_o)
{
}
