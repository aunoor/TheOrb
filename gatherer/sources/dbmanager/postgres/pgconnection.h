#ifndef PGCONNECTION_H
#define PGCONNECTION_H

#include "common/commondefs.h"

class PgConnection {
public:
    PgConnection(DBConnParams &params);
    bool Open();
};


#endif //PGCONNECTION_H
