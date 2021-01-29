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

//--------------------------------------------------------------------------------------------------------------------//

bool PgConnection::Open() {
    m_pgConn = PQconnectdb(m_connInfo.c_str());
    return (m_pgConn != nullptr);
}
