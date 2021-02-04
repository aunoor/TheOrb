#ifndef SPACEOBJECTS_H
#define SPACEOBJECTS_H

#include <ctime>
#include <string>
#include <list>

struct SStarCoords{
    float x = 0;
    float y = 0;
    float z = 0;
    bool IsValid = false;
};

struct Station {
    std::string Name;
    uint32_t Id = 0;
    uint64_t MarketId = 0;
    ///Type of station: Coriolis, Outpost, asteroid base...
    std::string Type;
    float Dist2Arrival = -1;
    bool HaveMarket = false;
    uint64_t SystemId64 = -1;

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
    uint32_t BuyPrice = 0;
    uint32_t Demand = 0;
    int32_t DemandBracket=-1;
    uint32_t MeanPrice = 0;
    std::string Name;
    uint32_t SellPrice = 0;
    uint32_t Stock = 0;
    int32_t StockBracket = -1;
};

struct MarketData {
    uint64_t MarketId;
    std::string StationName;
    std::string SystemName;
    std::tm Timestamp = {};
    std::list<Commodity> Commodities;
};

#endif //SPACEOBJECTS_H
