#include "pgconnection.h"

//--------------------------------------------------------------------------------------------------------------------//

PgConnection::PgConnection(DBConnParams &params) {
    m_pgConn = nullptr;

    //postgresql://[user[:password]@][netloc][:port][/dbname][?param1=value1&...]
    m_connInfo = "postgresql://";

    std::string userData;
    if (!params.user.empty()) {
        userData = params.user;
    }
    if (!params.password.empty() && !params.user.empty()) {
        userData += ":"+params.password;
    }
    if (!userData.empty()) {
        m_connInfo +=userData+"@";
    }

    m_connInfo += params.host;
    if (params.port>0) {
        m_connInfo += ":"+std::to_string(params.port);
    }

    m_connInfo += "/"+params.dbName;
}

PgConnection::~PgConnection() {
    if (m_pgConn) {
        PQfinish(m_pgConn);
    }
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgConnection::Open() {
    m_pgConn = PQconnectdb(m_connInfo.c_str());
    if(PQstatus(m_pgConn)!= CONNECTION_OK){
        //printf("Connection to database failed:%s\n", PQerrorMessage(m_pgConn));
    }

    return (PQstatus(m_pgConn) == CONNECTION_OK);
}

//--------------------------------------------------------------------------------------------------------------------//

PGconn *PgConnection::GetPGconn() {
    return m_pgConn;
}

bool PgConnection::CheckConnection() {
    if (m_pgConn == nullptr) return false;

    bool wasReset = false;
    ConnStatusType status;
    do {
        status = PQstatus(m_pgConn);
        if (status == CONNECTION_BAD) {
            if (wasReset) break;
            PQreset(m_pgConn);
            wasReset = true;
            continue;
        }
        break;
    } while(true);

    return (status == CONNECTION_OK);
}
