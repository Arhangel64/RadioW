#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>
#include <map>
#include <list>

#include "tables/table.h"
#include "tables/cmap.h"
#include "tables/hhea.h"
#include "tables/hmtx.h"
#include "tables/head.h"
#include "tables/name.h"

class Font
{
public:
    enum SfntVersion {
        TrueTypeOutlines,
        WithCFFData
    };
    Font(const std::string& p_path);
    ~Font();


    bool hasTable(const std::string& tag) const;
    Table* getTable(const std::string& tag);
    std::list<std::string> availableTables() const;
    std::map<uint32_t, uint32_t> getCharCodeToCIDTable(uint32_t start = 0, uint32_t end = 0xffff);
    std::map<uint32_t, Hmtx::HMetric> getCharCodeMetrics(uint32_t start = 0, uint32_t end = 0xffff);
    uint16_t getUnitsPerEm();
    int16_t getAscent();
    int16_t getDescent();
    int16_t getLineGap();
    int16_t getCaretSlopeRise();
    int16_t getCaretSlopeRun();
    int16_t getXMin();
    int16_t getXMax();
    int16_t getYMin();
    int16_t getYMax();
    std::string getNameField(std::string key);

    SfntVersion version;
    uint16_t numberTables;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;

private:
    const std::string path;
    std::map<std::string, Table*> tables;
    Cmap* cmap;
    Hhea* hhea;
    Hmtx* hmtx;
    Head* head;
    Name* name;
};

#endif // FILE_H
