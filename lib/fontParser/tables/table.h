#ifndef TABLE_H
#define TABLE_H

#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>

class Table
{
public:
    Table(const std::string& p_tag, uint32_t p_checkSum, uint32_t p_offset, uint32_t p_length);
    virtual ~Table();

    const std::string tag;
    const uint32_t checkSum;
    const uint32_t offset;
    const uint32_t length;

    static Table* fromIfStream(std::ifstream& stream);

    virtual void read(const std::string& path);
};


#endif // TABLE_H
