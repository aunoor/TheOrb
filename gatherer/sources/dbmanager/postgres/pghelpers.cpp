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
