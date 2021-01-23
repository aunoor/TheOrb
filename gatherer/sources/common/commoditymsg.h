#ifndef COMMODITYMSG_H
#define COMMODITYMSG_H

#include "msg_common.h"

#include <chrono>
#include <string>
#include <vector>

struct Commodity {
    uint32_t buyPrice; //"Price to buy from the market"
    uint32_t demand;
    uint32_t demandBracket;
    uint32_t meanPrice;
    std::string name; //"Symbolic name as returned by the Companion API"
    uint32_t sellPrice; //"Price to sell to the market"
    uint32_t stock;
    uint32_t stockBracket;
};

class CommodityMsg : public AbstractMsg {
public:
    MsgType GetType() override;

    void AddCommodity(Commodity &item);
    void SetMarketId(uint32_t id);
    void SetStationName(const std::string &name);
    void SetSystemName(const std::string &name);
    void SetTimeStamp(const std::string &dateTime);

private:
    std::vector<Commodity> m_commodities;
    uint32_t m_marketId;
    std::string m_stationName;
    std::string m_systemName;
    std::chrono::time_point<std::chrono::system_clock> m_timestamp;





};


#endif //COMMODITYMSG_H
