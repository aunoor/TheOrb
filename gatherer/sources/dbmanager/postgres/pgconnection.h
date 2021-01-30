#ifndef PGCONNECTION_H
#define PGCONNECTION_H

#include "common/commondefs.h"
#include <libpq-fe.h>

class PgConnection {
public:
    explicit PgConnection(DBConnParams &params);
    ~PgConnection();
    bool Open();
    PGconn * GetPGconn();
    bool CheckConnection();

private:
    std::string m_connInfo;
    PGconn *m_pgConn;
};


#endif //PGCONNECTION_H
