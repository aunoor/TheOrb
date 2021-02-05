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
        logMsg(LL_Critical, "PgProvider::GetSystemByName(): Can't get connection to DB.");
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
            logMsg(LL_Critical, asprintf("PgProvider::GetSystemByName(): SQL error: %s", pgQuery.ErrorString().c_str()));;
            break;
        }
        if (!pgQuery.Next()) break;
        PgRecord record = pgQuery.Record();
        starSystem = pgResult2StarSystem(&record);
    } while(false);

    if (starSystem.IsValid) {
        res = getStations(starSystem.Id64, starSystem.Stations, pgCon);
        if (!res) {
            starSystem.IsValid = false;
        }
    }

    m_connPool->ReturnConnection(pgCon);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgProvider::UpdateSystem(StarSystem &system) {
    auto pgCon = m_connPool->GetConnection();
    if (pgCon == nullptr) {
        logMsg(LL_Critical, "PgProvider::UpdateSystem(): Can't get connection to DB.");
        return false;
    }
    PgQuery pgQuery(pgCon);
    bool res;
    do {
        res = pgQuery.Exec("begin transaction;");
        if (!res) {
            logMsg(LL_Critical, asprintf("PgProvider::UpdateSystem(): SQL error: %s", pgQuery.ErrorString().c_str()));;
            break;
        }

        pgQuery.Clear();
        std::string query = "select id64 from systems where (id64 = :id64);";
        pgQuery.Prepare(query);
        pgQuery.BindValue(":id64", system.Id64);
        res = pgQuery.Exec();
        if (!res) {
            logMsg(LL_Critical, asprintf("PgProvider::UpdateSystem(): SQL error: %s", pgQuery.ErrorString().c_str()));;
            break;
        }
        if (pgQuery.RowCount()) {
            query = "update systems set system_name=:name, has_coords=:has_coords, x=:x, y=:y, z=:z, require_permit=:permit where (id64 = :id64);";
        } else {
            query = "insert into systems (id, id64, system_name, has_coords, x, y, z, require_permit) values (:id, :id64, :name, :has_coords, :x, :y, :z, :permit)";
        }

        pgQuery.Clear();
        pgQuery.Prepare(query);
        pgQuery.BindValue(":id", system.Id);
        pgQuery.BindValue(":id64", system.Id64);
        pgQuery.BindValue(":name", system.Name);
        pgQuery.BindValue(":has_coords", system.Coords.IsValid);
        pgQuery.BindValue(":x", system.Coords.x);
        pgQuery.BindValue(":y", system.Coords.y);
        pgQuery.BindValue(":z", system.Coords.z);
        pgQuery.BindValue(":permit", system.RequirePermit);
        res = pgQuery.Exec();
        if (!res) {
            logMsg(LL_Critical, asprintf("PgProvider::UpdateSystem(): SQL error: %s", pgQuery.ErrorString().c_str()));;
            break;
        }

        for (auto &station : system.Stations) {
            res = updateStation(station, system.Id64, pgCon);
            if (!res) break;
        }

        res = pgQuery.Exec("commit transaction;");
        if (!res) {
            logMsg(LL_Critical, asprintf("PgProvider::UpdateSystem(): SQL error: %s", pgQuery.ErrorString().c_str()));;
        }
    } while(false);

    if (!res) {
        pgQuery.Exec("rollback transaction;");
    }
    m_connPool->ReturnConnection(pgCon);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgProvider::UpdateMarketData(MarketData &marketData) {

    auto pgCon = m_connPool->GetConnection();
    if (pgCon == nullptr) {
        logMsg(LL_Critical, "PgProvider::UpdateMarketData(): Can't get connection to DB.");
        return false;
    }
    PgQuery pgQuery(pgCon);
    bool res;
    std::string query;
    do {
        res = pgQuery.Exec("begin transaction;");
        if (!res) {
            logMsg(LL_Critical, asprintf("PgProvider::UpdateMarketData(): SQL error: %s", pgQuery.ErrorString().c_str()));;
            break;
        }

        pgQuery.Clear();
        pgQuery.Prepare("delete from commodity where(market_id=:market_id);");
        pgQuery.BindValue(":market_id", marketData.MarketId);
        res = pgQuery.Exec();
        if (!res) {
            logMsg(LL_Critical, asprintf("PgProvider::UpdateMarketData(): SQL error: %s", pgQuery.ErrorString().c_str()));;
            break;
        }

        pgQuery.Clear();
        pgQuery.Prepare("INSERT INTO markets (market_id, updated) VALUES (:market_id, :updated) ON CONFLICT (market_id) DO UPDATE SET updated = EXCLUDED.updated;");
        pgQuery.BindValue(":market_id", marketData.MarketId);
        pgQuery.BindValue(":updated", marketData.Timestamp);
        res = pgQuery.Exec();
        if (!res) {
            logMsg(LL_Critical, asprintf("PgProvider::UpdateMarketData(): SQL error: %s", pgQuery.ErrorString().c_str()));;
            break;
        }

        for (const auto& item : marketData.Commodities) {
            pgQuery.Clear();
            pgQuery.Prepare("INSERT INTO commodity (market_id, commodity_id, mean_price, buy_price, stock, stock_bracket, sell_price, demand, demand_bracket) VALUES (:market_id, :commodity_id, :mean_price, :buy_price, :stock, :stock_bracket, :sell_price, :demand, :demand_bracket);");
            pgQuery.BindValue(":market_id", marketData.MarketId);
            pgQuery.BindValue(":commodity_id", item.Name);
            pgQuery.BindValue(":mean_price", item.MeanPrice);
            pgQuery.BindValue(":buy_price", item.BuyPrice);
            pgQuery.BindValue(":stock", item.Stock);
            pgQuery.BindValue(":stock_bracket", item.StockBracket);
            pgQuery.BindValue(":sell_price", item.SellPrice);
            pgQuery.BindValue(":demand", item.Demand);
            pgQuery.BindValue(":demand_bracket", item.DemandBracket);
            res = pgQuery.Exec();
            if (!res) {
                logMsg(LL_Critical, asprintf("PgProvider::UpdateMarketData(): SQL error: %s", pgQuery.ErrorString().c_str()));;
                break;
            }
        }
        if (!res) {
            break;
        }

        res = pgQuery.Exec("commit transaction;");
        if (!res) {
            logMsg(LL_Critical, asprintf("PgProvider::UpdateMarketData(): SQL error: %s", pgQuery.ErrorString().c_str()));;
        }
    } while(false);

    if (!res) {
        pgQuery.Exec("rollback transaction;");
    }
    m_connPool->ReturnConnection(pgCon);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgProvider::updateStation(Station &station, uint64_t systemId, PgConnection *pgCon) {
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
            logMsg(LL_Critical, asprintf("PgProvider::updateStation(): SQL error: %s", pgQuery.ErrorString().c_str()));;
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
        if (!res) {
            logMsg(LL_Critical, asprintf("PgProvider::updateStation(): SQL error: %s", pgQuery.ErrorString().c_str()));;
            break;
        }
    } while(false);

    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgProvider::getStations(uint64_t systemId, std::list<Station> &stations, PgConnection *pgCon) {
    bool res;
    std::string query;
    PgQuery pgQuery(pgCon);
    do {
        query = "select * from stations where (system_id64=:id64);";
        pgQuery.Prepare(query);
        pgQuery.BindValue(":id64", systemId);
        res = pgQuery.Exec();
        if (!res) {
            logMsg(LL_Critical, asprintf("PgProvider::getStations(): SQL error: %s", pgQuery.ErrorString().c_str()));;
            break;
        }

        while(pgQuery.Next()) {
            PgRecord record = pgQuery.Record();
            auto station = pgResult2Station(&record);
            if (!station.IsValid) {
                res = false;
                break;
            }
            stations.push_back(station);
        }

    } while(false);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

void PgProvider::logMsg(ESLogLevel msgType, const std::string &event) {
    if (LogMsg) {
        LogMsg(msgType, event);
    }
}
