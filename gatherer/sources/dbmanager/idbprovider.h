#ifndef IDBPROVIDER_H
#define IDBPROVIDER_H

#include "common/spaceobjects.h"
#include "common/commondefs.h"
#include <functional>
#include <string>

class IDBProvider {
public:
    virtual bool Init() = 0;
    virtual bool GetSystemByName(const std::string &name, StarSystem &starSystem) = 0;
    virtual bool UpdateSystem(StarSystem &system) = 0;
    virtual bool UpdateMarketData(MarketData &marketData) = 0;
    std::function<void(ESLogLevel msgType, const std::string &event)> LogMsg;
};


#endif //IDBPROVIDER_H
