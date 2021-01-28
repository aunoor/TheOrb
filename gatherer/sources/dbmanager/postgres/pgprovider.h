#ifndef PGPROVIDER_H
#define PGPROVIDER_H

#include "../idbprovider.h"

class PgConnPool;

class PgProvider : public IDBProvider {
public:
    PgProvider(DBConnParams &params);
    ~PgProvider();

    bool Init() override;

    StarSystem GetSystemByName(const std::string &name) override;

    bool UpdateSystem(StarSystem &system) override;

private:
    PgConnPool *m_connPool;
};


#endif //PGPROVIDER_H
