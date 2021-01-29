#ifndef PGCONNECTION_H
#define PGCONNECTION_H

#include "common/commondefs.h"
#include <libpq-fe.h>

class PgConnection {
public:
    explicit PgConnection(DBConnParams &params);
    bool Open();
private:
    std::string m_connInfo;
    PGconn *m_pgConn;
};


#endif //PGCONNECTION_H
