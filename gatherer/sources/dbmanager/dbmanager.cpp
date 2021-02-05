#include "dbmanager.h"
#include "idbprovider.h"

DBManager::DBManager(IDBProvider *provider) {
    m_provider = provider;
    m_provider->LogMsg = [](ESLogLevel msgType, const std::string &event) {
        SLogger::GetInstance()->LogMessage(msgType, "DBProvider", event);
    };
}

//--------------------------------------------------------------------------------------------------------------------//

bool DBManager::Init() {
    return m_provider->Init();
}

//--------------------------------------------------------------------------------------------------------------------//

bool DBManager::IsSystemsLoaded() {
    StarSystem starSystem;
    bool res = GetSystemByName("Cubeo", starSystem);
    return (res && starSystem.IsValid);
}

//--------------------------------------------------------------------------------------------------------------------//

bool DBManager::UpdateSystem(StarSystem &system) {
    return m_provider->UpdateSystem(system);
}

//--------------------------------------------------------------------------------------------------------------------//

bool DBManager::GetSystemByName(const std::string &name, StarSystem &starSystem) {
    bool res = m_provider->GetSystemByName(name, starSystem);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

bool DBManager::StoreMarketData(MarketData &marketData) {
    bool res = m_provider->UpdateMarketData(marketData);
    return res;
}
