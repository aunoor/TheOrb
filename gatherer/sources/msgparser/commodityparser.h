#ifndef COMMODITYPARSER_H
#define COMMODITYPARSER_H

#include <rapidjson/document.h>

class CommodityParser {
public:
    static void Parse(rapidjson::Document &document);
};


#endif //COMMODITYPARSER_H
