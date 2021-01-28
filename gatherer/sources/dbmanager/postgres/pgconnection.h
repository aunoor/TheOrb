#ifndef PGCONNECTION_H
#define PGCONNECTION_H

#include "../idbprovider.h"

class PgConnection {
public:
    PgConnection(DBConnParams &params);
    bool Open();
};


#endif //PGCONNECTION_H
