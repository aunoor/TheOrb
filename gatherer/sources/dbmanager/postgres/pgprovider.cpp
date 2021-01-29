#include "pgprovider.h"
#include "pgconnpool.h"

//--------------------------------------------------------------------------------------------------------------------//

PgProvider::PgProvider(DBConnParams params) {
    m_connPool = new PgConnPool(params);
}

//--------------------------------------------------------------------------------------------------------------------//

PgProvider::~PgProvider() {
    delete m_connPool;
}

bool PgProvider::Init() {
    return m_connPool->Init(3);
}

//--------------------------------------------------------------------------------------------------------------------//

StarSystem PgProvider::GetSystemByName(const std::string &name) {
    return StarSystem();
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgProvider::UpdateSystem(StarSystem &system) {
    return false;
}
