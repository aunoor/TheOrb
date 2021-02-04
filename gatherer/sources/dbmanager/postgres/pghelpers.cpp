#include "pghelpers.h"
#include "pgquery.h"

//--------------------------------------------------------------------------------------------------------------------//

StarSystem pgResult2StarSystem(PgRecord *pgRecord) {
    StarSystem starSystem{};
    if (pgRecord->IsValid()) {
        starSystem.Id = std::stoul(pgRecord->Value("id"));
        starSystem.Id64 = std::stoull(pgRecord->Value("id64"));
        starSystem.Name = pgRecord->Value("system_name");
        starSystem.Coords.x = std::stof(pgRecord->Value("x"));
        starSystem.Coords.y = std::stof(pgRecord->Value("y"));
        starSystem.Coords.z = std::stof(pgRecord->Value("z"));
        starSystem.RequirePermit = false;
        if (!pgRecord->isNull("require_permit")) {
            starSystem.RequirePermit = (pgRecord->Value("require_permit") == "true");
        }
        starSystem.IsValid = true;
    }
    return starSystem;
}

//--------------------------------------------------------------------------------------------------------------------//
/*
	have_market boolean NULL
 */

Station pgResult2Station(PgRecord *pgRecord) {
    Station station{};
    if (pgRecord->IsValid()) {
        station.Id = std::stoul(pgRecord->Value("station_id"));
        station.Name = pgRecord->Value("station_name");
        station.MarketId = std::stoul(pgRecord->Value("market_id"));
        station.Type = pgRecord->Value("station_type");
        station.Dist2Arrival = std::stof(pgRecord->Value("distance2arrival"));
        station.HaveMarket = false;
        if (!pgRecord->isNull("have_market")) {
            station.HaveMarket = (pgRecord->Value("have_market") == "t");
        }
        station.IsValid = true;
    }
    return station;
}
