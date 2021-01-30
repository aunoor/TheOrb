#ifndef PGPROVIDER_H
#define PGPROVIDER_H

#include "common/commondefs.h"
#include "dbmanager/idbprovider.h"

class PgConnPool;

class PgProvider : public IDBProvider {
public:
    explicit PgProvider(DBConnParams params);
    ~PgProvider();

    bool Init() override;

    bool GetSystemByName(const std::string &name, StarSystem &starSystem) override;

    bool UpdateSystem(StarSystem &system) override;

private:
    PgConnPool *m_connPool;
};


#endif //PGPROVIDER_H
