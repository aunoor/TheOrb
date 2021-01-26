#ifndef SPACEOBJECTS_H
#define SPACEOBJECTS_H

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
    float Dist2Arrival = 0;
    bool HaveMarket = false;
};


struct StarSystem {
    uint32_t Id = 0;
    uint64_t Id64 = 0;
    std::string Name;
    SStarCoords Coords{};
    std::list<Station> Stations;
};

#endif //SPACEOBJECTS_H
