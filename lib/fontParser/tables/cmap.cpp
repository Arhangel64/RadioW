#include "cmap.h"
#include <arpa/inet.h>

CMap::CMap(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length):
    Table(p_tag, p_checkSum, p_offset, p_length),
    initialized(false),
    mt(0)
{
}

CMap::~CMap()
{
    if (initialized) {
        delete mt;
    }
}


void CMap::read(const std::string& path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    file.seekg(offset);
    
    char * buffer;
    buffer = new char[2];
    
    file.read(buffer, 2);
    version = ntohs(*((uint16_t*) buffer));
    
    file.read(buffer, 2);
    numberOfTables = ntohs(*((uint16_t*) buffer));
    
    delete[] buffer;
    buffer = new char[8];
    
    std::list<Enc> encodings;
    for (int i = 0; i < numberOfTables; ++i) {
        file.read(buffer, 8);
        
        char pb[2] = {buffer[0], buffer[1]};
        char eb[2] = {buffer[2], buffer[3]};
        char ob[4] = {buffer[4], buffer[5], buffer[6], buffer[7]};
        
        uint16_t pid = ntohs(*((uint16_t*) pb));
        uint16_t eid = ntohs(*((uint16_t*) eb));
        uint16_t offset = ntohl(*((uint32_t*) ob));
        
        //std::cout << "Found encoding platformId " << pid << ", encodingId " << eid << std::endl;
        
        if (pid == 0) {
            encodings.emplace_back(pid, eid, offset);
        }
    }
    delete[] buffer;
    std::list<Enc>::const_iterator itr = encodings.begin();
    std::list<Enc>::const_iterator end = encodings.end();
    for (; itr != end; ++itr) {
        //std::cout << "Trying platformId " << itr->platformId << ", encodingId " << itr->encodingId << std::endl;
        file.seekg(offset + itr->offset);
        bool success = true;
        MappingTable* table;
        try {
            table = MappingTable::fromIfStream(file);
        } catch (int e) {
            success = false;
        }
        if (success) {
            initialized = true;
            mt = table;
            break;
        }
    }
    file.close();
    if (!initialized) {
        //std::cout << "Error reading cmap: no supported encoding format" << std::endl;
        throw 3;
    }
}

uint32_t CMap::getCID(uint32_t charCode) const
{
    return this->mt->getCID(charCode);
}


MappingTable * MappingTable::fromIfStream(std::ifstream& file)
{
    uint64_t position = file.tellg();
    char * buffer;
    buffer = new char[2];
    file.read(buffer, 2);
    uint16_t format = ntohs(*((uint16_t*) buffer));
    
    MappingTable* table = NULL;
    
    if (format >= 8) {
        if (format != 14) {
            file.read(buffer, 2);       //padded .0 in stupid formats
        }
        delete[] buffer;
        buffer = new char[4];
        file.read(buffer, 4);
        uint32_t length = ntohl(*((uint32_t*) buffer));
        file.seekg(position);
        buffer = new char[length];
        file.read(buffer, length);
    } else {
        file.read(buffer, 2);
        uint16_t length = ntohs(*((uint16_t*) buffer));
        file.seekg(position);
        buffer = new char[length];
        file.read(buffer, length);
        
        if (format == 4) {
            table = new Format4(buffer, length);
        }
    }
    
    delete[] buffer;
    
    if (table == NULL) {
        std::cout << "Unrecognized format " << format << std::endl;
        throw 3;
    }
    return table;
}

MappingTable::MappingTable(uint16_t p_f):
    format(p_f)
{
    
}

MappingTable::~MappingTable()
{
}


Format4::Format4(char * data, uint16_t length):
    MappingTable(4),
    charCodesEndCode(),
    segments(0),
    glyphIndexArray(0)
{
    char sc[2] = {data[6], data[7]};
    uint16_t segCount = ntohs(*((uint16_t*) sc)) / 2;
    segments = new std::vector<SegParams>(segCount);
    
    int endCodeShift = 14;
    int startCodeShift = endCodeShift + segCount * 2 + 2;
    int deltaShift = startCodeShift + segCount * 2;
    int rangeShift = deltaShift + segCount * 2;
    int giaShift = rangeShift + segCount * 2;
    int giaLength = (length - giaShift) / 2;
    glyphIndexArray = new std::vector<uint16_t>(giaLength);
//     std::cout << "Segments: " << segCount << ", ";
//     std::cout << "Glyphs: " << giaLength << "\n";
//     std::cout << "******************************************" << "\n";
    
    for (int i = 0; i < segCount; ++i) {
        char cc[2] = {data[2 * i + endCodeShift], data[2 * i + endCodeShift + 1]};
        char sc[2] = {data[2 * i + startCodeShift], data[2 * i + startCodeShift + 1]};
        char dc[2] = {data[2 * i + deltaShift], data[2 * i + deltaShift + 1]};
        char rc[2] = {data[2 * i + rangeShift], data[2 * i + rangeShift + 1]};
        
        uint16_t endCharCode = ntohs(*((uint16_t*) cc));
        uint16_t startCharCode = ntohs(*((uint16_t*) sc));
        int16_t delta = ntohs(*((int16_t*) dc));
        uint16_t range = ntohs(*((uint16_t*) rc));
        
        SegParams& sp = segments->at(i);
        sp.endCode = endCharCode;
        sp.startCode = startCharCode;
        sp.idDelta = delta;
        sp.idRangeOffset = range;
        
        charCodesEndCode.insert(std::make_pair(endCharCode, i));
//         std::cout << "Segment " << i << ",\t";
//         std::cout << "Start " << startCharCode << ",\t";
//         std::cout << "End " << endCharCode << ",\t";
//         std::cout << "Delta " << delta << ",\t";
//         std::cout << "Range " << range << "\n";
    }
//     std::cout << "******************************************" << std::endl;;
    
    for (int i = 0; i < giaLength; ++i) {
        char cc[2] = {data[2 * i + giaShift], data[2 * i + giaShift + 1]};
        uint16_t glyphIndex = ntohs(*((uint16_t*) cc));
        glyphIndexArray->at(i) = glyphIndex;
    }
}

Format4::~Format4()
{
    delete segments;
    delete glyphIndexArray;
}


uint32_t Format4::getCID(uint32_t charCode) const
{
    uint16_t cid;
    uint16_t c = charCode & 0xffff;
    std::map<uint16_t, uint16_t>::const_iterator itr = charCodesEndCode.lower_bound(c);
    uint16_t i = itr->second;
    SegParams& seg = segments->at(i);
    if (seg.startCode > c) {
        return 0;
    }
    
    if (seg.idRangeOffset == 0) {
        cid = c + seg.idDelta;
    } else {
        cid = i + seg.idRangeOffset - segments->size() + c - seg.startCode;
    }
    
    return cid;
}
