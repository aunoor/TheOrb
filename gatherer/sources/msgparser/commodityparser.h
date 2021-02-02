#ifndef COMMODITYPARSER_H
#define COMMODITYPARSER_H

#include <rapidjson/document.h>

class CommodityParser {
public:
    static bool Parse(rapidjson::Document &document);
};


#endif //COMMODITYPARSER_H
