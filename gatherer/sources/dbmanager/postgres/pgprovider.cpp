#include "pgprovider.h"
#include "pgconnpool.h"
#include "pghelpers.h"
#include "pgquery.h"

#include <libpq-fe.h>

//--------------------------------------------------------------------------------------------------------------------//

bool updateStation(Station &station, uint64_t systemId, PgConnection *pgCon);

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
    PgQuery pgQuery(pgCon);
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
    PgQuery pgQuery(pgCon);
    bool res;
    do {
        res = pgQuery.Exec("begin transaction;");
        if (!res) {
            //TODO: log
            break;
        }

        pgQuery.Clear();
        std::string query = "select id64 from systems where (id64 = :id64);";
        pgQuery.Prepare(query);
        pgQuery.BindValue(":id64", system.Id64);
        res = pgQuery.Exec();
        if (!res) {
            //TODO: log
            break;
        }
        if (pgQuery.RowCount()) {
            query = "update systems set system_name=:name, x=:x, y=:y, z=:z, require_permit=:permit where (id64 = :id64);";
        } else {
            query = "insert into systems (id, id64, system_name, x, y, z, require_permit) values (:id, :id64, :name, :x, :y, :z, :permit)";
        }

        pgQuery.Clear();
        pgQuery.Prepare(query);
        pgQuery.BindValue(":id", system.Id);
        pgQuery.BindValue(":id64", system.Id64);
        pgQuery.BindValue(":name", system.Name);
        pgQuery.BindValue(":x", system.Coords.x);
        pgQuery.BindValue(":y", system.Coords.y);
        pgQuery.BindValue(":z", system.Coords.z);
        pgQuery.BindValue(":permit", system.RequirePermit);
        res = pgQuery.Exec();
        if (!res) {
            //TODO: log
            break;
        }

        //TODO: store stations
        for (auto &station : system.Stations) {
            res = updateStation(station, system.Id64, pgCon);
            if (!res) break;
        }

        res = pgQuery.Exec("commit transaction;");
    } while(false);

    if (!res) {
        pgQuery.Exec("rollback transaction;");
    }
    m_connPool->ReturnConnection(pgCon);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

bool updateStation(Station &station, uint64_t systemId, PgConnection *pgCon) {
    bool res;
    std::string query;
    PgQuery pgQuery(pgCon);
    do {
        query = "select station_id from stations where (system_id64=:id64 and station_id=:id);";
        pgQuery.Prepare(query);
        pgQuery.BindValue(":id64", systemId);
        pgQuery.BindValue(":id", station.Id);
        res = pgQuery.Exec();
        if (!res) {
            break;
        }

        if (pgQuery.RowCount()) {
            query = "update stations set station_name=:name, market_id=:marketId, station_type=:type, distance2arrival=:dist, have_market=:haveMarket where (station_id=:id);";
        } else {
            query = "insert into stations (system_id64, station_id, station_name, market_id, station_type, distance2arrival, have_market) values (:systemId64, :id, :name, :marketId, :type, :dist, :haveMarket);";
        }
        pgQuery.Clear();
        pgQuery.Prepare(query);
        pgQuery.BindValue(":id", station.Id);
        pgQuery.BindValue(":systemId64", systemId);
        pgQuery.BindValue(":name", station.Name);
        pgQuery.BindValue(":marketId", station.MarketId);
        pgQuery.BindValue(":type", station.Type);
        pgQuery.BindValue(":dist", station.Dist2Arrival);
        pgQuery.BindValue(":haveMarket", station.HaveMarket);
        res = pgQuery.Exec();
    } while(false);

    return res;
}
