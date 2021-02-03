#ifndef COMMODITYPARSER_H
#define COMMODITYPARSER_H

#include <rapidjson/document.h>
#include "common/spaceobjects.h"

class CommodityParser {
public:
    static bool Parse(rapidjson::Value &message, MarketData &marketData);
};


#endif //COMMODITYPARSER_H
