#ifndef PGCONNPOOL_H
#define PGCONNPOOL_H

#include "../idbprovider.h"
#include "pgconnection.h"

#include <cstdint>
#include <list>

class PgConnPool {
public:
    explicit PgConnPool(DBConnParams &params);
    ~PgConnPool();
    bool Init(uint8_t connCount);
private:
    DBConnParams m_params;
    std::list<PgConnection*> m_connections;
};


#endif //PGCONNPOOL_H
