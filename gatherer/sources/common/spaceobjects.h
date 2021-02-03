#ifndef SPACEOBJECTS_H
#define SPACEOBJECTS_H

#include <ctime>
#include <string>
#include <list>

struct SStarCoords{
    float x;
    float y;
    float z;
};

struct Station {
    std::string Name;
    uint32_t Id = 0;
    uint64_t MarketId = 0;
    ///Type of station: Coriolis, Outpost, asteroid base...
    std::string Type;
    float Dist2Arrival = -1;
    bool HaveMarket = false;

    bool IsValid = false;
};


struct StarSystem {
    uint32_t Id = 0;
    uint64_t Id64 = 0;
    std::string Name;
    SStarCoords Coords{};
    bool RequirePermit = false;
    std::list<Station> Stations;

    bool IsValid = false;
};

struct Commodity {
    uint32_t BuyPrice;
    uint32_t Demand;
    uint32_t DemandBracket;
    uint32_t MeanPrice;
    std::string Name;
    uint32_t SellPrice;
    uint32_t Stock;
    uint32_t StockBracket;
};

struct MarketData {
    uint64_t MarketId;
    std::string StationName;
    std::string SystemName;
    std::tm Timestamp;
    std::list<Commodity> Commodities;
};

#endif //SPACEOBJECTS_H
