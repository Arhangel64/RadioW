#include "font.h"
#include <iostream>
#include <list>
#include <string>
#include "tables/hmtx.h"

int main(int argc, char **argv) {
    Font file(argv[1]);
    
    std::map<uint32_t, Hmtx::HMetric> cidMap = file.getCharCodeMetrics(0, 0x4ff);
    std::map<uint32_t, Hmtx::HMetric>::const_iterator itr = cidMap.begin();
    std::map<uint32_t, Hmtx::HMetric>::const_iterator end = cidMap.end();
    
    std::cout << "{\n";
    std::cout << "    \"ascent\": " << file.getAscent() << ",\n";
    std::cout << "    \"descent\": " << file.getDescent() << ",\n";
    std::cout << "    \"lineGap\": " << file.getLineGap() << ",\n";
    std::cout << "    \"caretSlopeRise\": " << file.getCaretSlopeRise() << ",\n";
    std::cout << "    \"caretSlopeRun\": " << file.getCaretSlopeRun() << ",\n";
    std::cout << "    \"unitsPerEm\": " << file.getUnitsPerEm() << ",\n";
    std::cout << "    \"fontFamily\": \"" << file.getNameField("fontFamily") << "\",\n";
    std::cout << "    \"postScriptName\": \"" << file.getNameField("postScriptName") << "\",\n";
    
    std::cout << "    \"boundingBox\": {\n";
    std::cout << "        \"xMin\": " << file.getXMin() << ",\n";
    std::cout << "        \"xMax\": " << file.getXMax() << ",\n";
    std::cout << "        \"yMin\": " << file.getYMin() << ",\n";
    std::cout << "        \"yMax\": " << file.getYMax() << "\n";
    std::cout << "    },\n";
    
    std::cout << "    \"advanceWidthArray\": [\n        ";
    int i = 0;
    for (; itr != end; ++itr) {
        if (i != 0) {
            if (i == 16) {
                std::cout << ",\n        ";
                i = 0;
            } else {
                std::cout << ", ";
            }
        }
        std::cout  << itr->second.advanceWidth;
        ++i;
    }
    
    std::cout << "\n    ]\n";
    std::cout << "}" << std::endl;
    
    return 0;
}
