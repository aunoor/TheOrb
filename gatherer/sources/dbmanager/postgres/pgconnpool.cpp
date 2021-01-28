#include "pgconnpool.h"

//--------------------------------------------------------------------------------------------------------------------//

PgConnPool::PgConnPool(DBConnParams &params) {
    m_params = params;
}

//--------------------------------------------------------------------------------------------------------------------//

PgConnPool::~PgConnPool() {
    for (auto conn : m_connections) {
        delete conn;
    }
    m_connections.clear();
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgConnPool::Init(uint8_t connCount) {
    for (int i=0; i<connCount; i++) {
        auto conn = new PgConnection(m_params);
        if (!conn->Open()) {
            return false;
        }
        m_connections.push_back(conn);
    }
    return true;
}

//--------------------------------------------------------------------------------------------------------------------//