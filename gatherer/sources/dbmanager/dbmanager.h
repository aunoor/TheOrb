#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <string>

class IDBProvider;

class DBManager {
public:
    explicit DBManager(IDBProvider *provider);
    bool Init();
    ///Function check if systems table is preloaded by systems
    bool IsSystemsLoaded();

private:
    IDBProvider *m_provider;
};


#endif //DBMANAGER_H
