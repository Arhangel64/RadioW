#include "font.h"
#include <arpa/inet.h>

Font::Font(const std::string& p_path):
    path(p_path),
    tables(),
    cmap(0),
    hhea(0),
    hmtx(0),
    head(0)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    
    char * buffer;
    
    buffer = new char[4];
    file.read(buffer, 4);
    uint32_t sfntVersion = ntohl(*((uint32_t*) buffer));
    if (sfntVersion == 0x00010000) {
        version = TrueTypeOutlines;
    } else if (sfntVersion == 0x4f54544f) {
        version = WithCFFData;
    } else {
        std::cout << "unsupported sfntVersion" << std::endl;
        throw 1;
    }
    delete[] buffer;
    
    buffer = new char[2];
    file.read(buffer, 2);
    numberTables = ntohs(*((uint16_t*) buffer));
    
    file.read(buffer, 2);
    searchRange = ntohs(*((uint16_t*) buffer));
    
    file.read(buffer, 2);
    entrySelector = ntohs(*((uint16_t*) buffer));
    
    file.read(buffer, 2);
    rangeShift = ntohs(*((uint16_t*) buffer));
    
    for (int i = 0; i < numberTables; ++i) {
        Table* t = Table::fromIfStream(file);
        tables.insert(std::make_pair(t->tag, t));
    }
    
    file.close();
}

Font::~Font()
{
    std::map<std::string, Table*>::const_iterator beg = tables.begin();
    std::map<std::string, Table*>::const_iterator end = tables.end();
    
    for (; beg != end; ++beg) {
        delete beg->second;
    }
}


bool Font::hasTable(const std::string& tag) const
{
    std::map<std::string, Table*>::const_iterator itr = tables.find(tag);
    return itr != tables.end();
}

std::list<std::string> Font::availableTables() const
{
    std::list<std::string> res;
    std::map<std::string, Table*>::const_iterator beg = tables.begin();
    std::map<std::string, Table*>::const_iterator end = tables.end();
    
    for (; beg != end; ++beg) {
        res.push_back(beg->first);
    }
    
    return res;
}

std::map<uint32_t, uint32_t> Font::getCharCodeToCIDTable(uint32_t start, uint32_t end)
{
    if (cmap == NULL) {
        cmap = static_cast<CMap*>(tables.at("cmap"));
        cmap->read(path);
    }
    std::map<uint32_t, uint32_t> res;
    for (uint32_t i = start; i <= end; ++i) {
        res.insert(std::make_pair(i, cmap->getCID(i)));
    }
    return res;
}

std::map<uint32_t, HMtx::HMetric> Font::getCharCodeMetrics(uint32_t start, uint32_t end)
{
    std::map<uint32_t, uint32_t> CCtoCID = getCharCodeToCIDTable(start, end);
    std::map<uint32_t, HMtx::HMetric> res;
    
    if (hmtx == NULL) {
        hmtx = static_cast<HMtx*>(tables.at("hmtx"));
        if (hhea == NULL) {
            hhea = static_cast<HHea*>(tables.at("hhea"));
            hhea->read(path);
        }
        hmtx->numOfLongHorMetrics = hhea->numOfLongHorMetrics;
        hmtx->read(path);
    }
    
    std::map<uint32_t, uint32_t>::const_iterator itr = CCtoCID.begin();
    std::map<uint32_t, uint32_t>::const_iterator mend = CCtoCID.end();
    
    for (; itr != mend; ++itr) {
        res.insert(std::make_pair(itr->first, hmtx->getMetric(itr->second)));
    }
    
    return res;
}


Table * Font::getTable(const std::string& tag)
{
    std::map<std::string, Table*>::iterator itr = tables.find(tag);
    return itr->second;
}

uint16_t Font::getUnitsPerEm()
{
    if (head == NULL) {
        head = static_cast<HEad*>(tables.at("head"));
        head->read(path);
    }
    return head->unitsPerEm;
}

int16_t Font::getAscent()
{
    if (hhea == NULL) {
        hhea = static_cast<HHea*>(tables.at("hhea"));
        hhea->read(path);
    }
    return hhea->ascent;
}

int16_t Font::getDescent()
{
    if (hhea == NULL) {
        hhea = static_cast<HHea*>(tables.at("hhea"));
        hhea->read(path);
    }
    return hhea->descent;
}

int16_t Font::getLineGap()
{
    if (hhea == NULL) {
        hhea = static_cast<HHea*>(tables.at("hhea"));
        hhea->read(path);
    }
    return hhea->lineGap;
}
