#include "dbmanager.h"
#include "idbprovider.h"

DBManager::DBManager(IDBProvider *provider) {
    m_provider = provider;
}

//--------------------------------------------------------------------------------------------------------------------//

bool DBManager::Init() {
    return m_provider->Init();
}

//--------------------------------------------------------------------------------------------------------------------//

bool DBManager::IsSystemsLoaded() {
    auto cubeo = GetSystemByName("Cubeo");
    return cubeo.IsValid;
}

//--------------------------------------------------------------------------------------------------------------------//

bool DBManager::UpdateSystem(StarSystem &system) {
    return m_provider->UpdateSystem(system);
}

//--------------------------------------------------------------------------------------------------------------------//

StarSystem DBManager::GetSystemByName(const std::string &name) {
    return m_provider->GetSystemByName(name);
}
