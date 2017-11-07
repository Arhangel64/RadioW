#include "table.h"
#include <arpa/inet.h>

#include "cmap.h"
#include "hhea.h"
#include "hmtx.h"
#include "head.h"
#include "name.h"

Table::Table(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length):
    tag(p_tag),
    checkSum(p_checkSum),
    offset(p_offset),
    length(p_length)
{
}

Table::~Table()
{
}

Table* Table::fromIfStream(std::ifstream& stream)
{
    char * buffer;
    buffer = new char[4];
    stream.read(buffer, 4);
    std::string tag(buffer, 4);
    
    stream.read(buffer, 4);
    uint32_t cs = ntohl(*((uint32_t*) buffer));
    
    stream.read(buffer, 4);
    uint32_t offset = ntohl(*((uint32_t*) buffer));
    
    stream.read(buffer, 4);
    uint32_t l = ntohl(*((uint32_t*) buffer));
    
    if          (tag == "cmap") {
        return new Cmap(tag, cs, offset, l);
    } else if   (tag == "hhea") {
        return new Hhea(tag, cs, offset, l);
    } else if   (tag == "hmtx") {
        return new Hmtx(tag, cs, offset, l);
    } else if   (tag == "head") {
        return new Head(tag, cs, offset, l);
    } else if   (tag == "name") {
        return new Name(tag, cs, offset, l);
    } else {
        return new Table(tag, cs, offset, l);
    }
}

void Table::read(const std::string& path)
{
    std::cout << "table with type " << tag << " is not supported yet" << std::endl;
}
