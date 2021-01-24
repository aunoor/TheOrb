#include "dbmanager.h"

DBManager::DBManager(IDBProvider *provider) {
    m_provider = provider;
}

bool DBManager::Init() {
    return false;
}

bool DBManager::IsSystemsLoaded() {
    return false;
}
