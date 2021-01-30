#ifndef PGCONNPOOL_H
#define PGCONNPOOL_H

#include "dbmanager/idbprovider.h"
#include "pgconnection.h"

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <list>
#include <mutex>
#include <thread>

class PgConnection;

class PgConnPool {
public:
    explicit PgConnPool(DBConnParams &params, uint8_t maxConn);
    ~PgConnPool();
    bool Init();
    PgConnection *GetConnection();
    void ReturnConnection(PgConnection* connection);
    
private:
    struct SConnInfo {
        ~SConnInfo() {
            delete connection;
        }
        PgConnection* connection = nullptr;
        bool isBusy = false;
    };

    ///Mutex for guard connections list
    std::mutex m_connMutex;
    /// Condition variable for regulate access to connections
    std::condition_variable cv;

    /// Max count of opened connections
    uint8_t m_maxConn;

    DBConnParams m_params;
    std::list<SConnInfo*> m_connections;

    std::atomic<bool> m_loopBreak{};

    std::thread *m_thread;
    ///Thread loop function
    void threadFunction();

    SConnInfo *createConnection();
    void removeConnection(SConnInfo* connInfo);
};


#endif //PGCONNPOOL_H
