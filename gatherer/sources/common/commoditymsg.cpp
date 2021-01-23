#include "commoditymsg.h"

MsgType CommodityMsg::GetType() {
    return mtCommodity;
}

void CommodityMsg::AddCommodity(Commodity &item) {
    m_commodities.push_back(item);
}

void CommodityMsg::SetMarketId(uint32_t id) {
    m_marketId = id;
}

void CommodityMsg::SetStationName(const std::string &name) {
    m_stationName = name;
}

void CommodityMsg::SetSystemName(const std::string &name) {
    m_systemName = name;
}

void CommodityMsg::SetTimeStamp(const std::string &dateTime) {
//    "timestamp": "2021-01-22T12:43:45Z"
}
