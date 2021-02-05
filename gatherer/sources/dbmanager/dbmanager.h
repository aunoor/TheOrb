#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "common/spaceobjects.h"
#include "logger/slogger.h"
#include <functional>
#include <string>

class IDBProvider;

class DBManager {
public:
    explicit DBManager(IDBProvider *provider);
    bool Init();
    ///Function check if systems table is preloaded by systems
    bool IsSystemsLoaded();

    bool UpdateSystem(StarSystem &system);

    bool GetSystemByName(const std::string &name, StarSystem &starSystem);

    bool StoreMarketData(MarketData &marketData);

private:
    IDBProvider *m_provider;
};


#endif //DBMANAGER_H
