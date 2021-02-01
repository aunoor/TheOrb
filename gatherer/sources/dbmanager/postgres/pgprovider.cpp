#include "pgprovider.h"
#include "pgconnpool.h"
#include "pghelpers.h"
#include "pgquery.h"

#include <libpq-fe.h>

//--------------------------------------------------------------------------------------------------------------------//

PgProvider::PgProvider(DBConnParams params) {
    m_connPool = new PgConnPool(params, 3);
}

//--------------------------------------------------------------------------------------------------------------------//

PgProvider::~PgProvider() {
    delete m_connPool;
}

bool PgProvider::Init() {
    return m_connPool->Init();
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgProvider::GetSystemByName(const std::string &name, StarSystem &starSystem) {
    starSystem.IsValid = false;
    auto tmpStarSystem  = StarSystem();

    auto pgCon = m_connPool->GetConnection();
    if (pgCon == nullptr) {
        return false;
    }

    std::string query =  "select * from systems where (system_name = :name);";
    PgSelectQuery pgQuery(pgCon);
    pgQuery.Prepare(query);
    pgQuery.BindValue(":name", name);
    bool res;
    do {
        res = pgQuery.Exec();
        if (!res) {
            printf(pgQuery.ErrorString().c_str());
            break;
        }
        if (!pgQuery.Next()) break;
        PgRecord record = pgQuery.Record();
        starSystem = pgResult2StarSystem(&record);
    } while(false);

    m_connPool->ReturnConnection(pgCon);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgProvider::UpdateSystem(StarSystem &system) {
    auto pgCon = m_connPool->GetConnection();
    if (pgCon == nullptr) {
        return false;
    }

    std::string query = "insert into systems (id, id64, system_name, x, y, z, require_permit) values (:id, :id64, :name, :x, :y, :z, :permit)";
    PgSelectQuery pgQuery(pgCon);
    pgQuery.Prepare(query);
    pgQuery.BindValue(":id", system.Id);
    pgQuery.BindValue(":id64", system.Id64);
    pgQuery.BindValue(":name", system.Name);
    pgQuery.BindValue(":x", system.Coords.x);
    pgQuery.BindValue(":y", system.Coords.y);
    pgQuery.BindValue(":z", system.Coords.z);
    pgQuery.BindValue(":permit", system.RequirePermit);
    bool res = pgQuery.Exec();

    m_connPool->ReturnConnection(pgCon);
    return res;
}
