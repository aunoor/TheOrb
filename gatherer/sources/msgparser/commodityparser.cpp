#include "commodityparser.h"
#include "common/schemadefs.h"

bool CommodityParser::Parse(rapidjson::Document &document) {
    if (!document.HasMember(headerName)) {
        //TODO: log
        return false;
    }
    if (!document.HasMember(messageName)) {
        //TODO: log
        return false;
    }

    auto message = document[messageName].GetObject();
    if (!message.HasMember(commoditiesName)) {
        //TODO: log
        return false;
    }

    if (!message[commoditiesName].IsArray()) {
        //TODO: log
        return false;
    }

    auto commodities = message[commoditiesName].IsArray();
    

}
