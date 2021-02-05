#ifndef PGPROVIDER_H
#define PGPROVIDER_H

#include "common/commondefs.h"
#include "dbmanager/idbprovider.h"

class PgConnPool;
class PgConnection;

class PgProvider : public IDBProvider {
public:
    explicit PgProvider(DBConnParams params);
    ~PgProvider();

    bool Init() override;

    bool GetSystemByName(const std::string &name, StarSystem &starSystem) override;

    bool UpdateSystem(StarSystem &system) override;

    bool UpdateMarketData(MarketData &marketData) override;

private:
    PgConnPool *m_connPool;
    void logMsg(ESLogLevel msgType, const std::string &event);
    bool updateStation(Station &station, uint64_t systemId, PgConnection *pgCon);
    bool getStations(uint64_t systemId, std::list<Station> &stations, PgConnection *pgCon);
};


#endif //PGPROVIDER_H
