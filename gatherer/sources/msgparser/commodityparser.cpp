#include "commodityparser.h"
#include "common/schemadefs.h"

void CommodityParser::Parse(rapidjson::Document &document) {
    if (!document.HasMember(headerName)) {
        //TODO: log
        return;
    }





}
