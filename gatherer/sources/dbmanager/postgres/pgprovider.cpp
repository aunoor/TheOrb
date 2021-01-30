#include "pgprovider.h"
#include "pgconnpool.h"
#include "pghelpers.h"

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

    std::string query = "select * from systems where (system_name = '"+name+"');";
    auto pgResult = PQexec(pgCon->GetPGconn(), query.c_str());

    bool res = checkSelectPgResult(pgResult, nullptr);
    if (res) {
        int cnt = PQntuples(pgResult);
        if (cnt != 0) {
            tmpStarSystem = pgResult2StarSystem(pgResult, 0);
        }
        tmpStarSystem.IsValid = true;
        starSystem = tmpStarSystem;
    }

    PQclear(pgResult);
    m_connPool->ReturnConnection(pgCon);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgProvider::UpdateSystem(StarSystem &system) {
    auto pgCon = m_connPool->GetConnection();
    if (pgCon == nullptr) {
        return false;
    }

    //std::string query = "insert into systems ()"


    m_connPool->ReturnConnection(pgCon);
    return false;
}
