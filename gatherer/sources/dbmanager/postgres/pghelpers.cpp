#include "pghelpers.h"

//--------------------------------------------------------------------------------------------------------------------//

StarSystem pgResult2StarSystem(PGresult *pgResult, int row) {
    StarSystem starSystem{};
    for (int i = 0; i < PQnfields(pgResult); i++) {
        std::string fldName = PQfname(pgResult, i);
        if (fldName == "id") {
            starSystem.Id = std::stol(PQgetvalue(pgResult, row, i));
        }
        if (fldName == "id64") {
            starSystem.Id64 = std::stoll(PQgetvalue(pgResult, row, i));
        }
        if (fldName == "system_name") {
            starSystem.Name = PQgetvalue(pgResult, row, i);
        }
        if (fldName == "x") {
            starSystem.Coords.x = std::stof(PQgetvalue(pgResult, row, i));
        }
        if (fldName == "y") {
            starSystem.Coords.y = std::stof(PQgetvalue(pgResult, row, i));
        }
        if (fldName == "z") {
            starSystem.Coords.z = std::stof(PQgetvalue(pgResult, row, i));
        }
        if (fldName == "require_permit") {
            std::string value = PQgetvalue(pgResult, row, i);
            starSystem.RequirePermit = (value == "true");
        }
    }
    return starSystem;
}

//--------------------------------------------------------------------------------------------------------------------//

bool checkSelectPgResult(PGresult *pgResult, std::string *error) {
    bool res = false;
    do {
        if (!pgResult) {
            break;
        }

        auto result = PQresultStatus(pgResult);
        if (PQresultStatus(pgResult) != PGRES_TUPLES_OK) {
            if (error) {
                *error = PQresultErrorMessage(pgResult);
            }
            break;
        }
        res = true;
    } while(false);
    return res;
}
