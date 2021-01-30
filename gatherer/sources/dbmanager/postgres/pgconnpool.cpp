#include "pgconnpool.h"
#include <thread>

//--------------------------------------------------------------------------------------------------------------------//

PgConnPool::PgConnPool(DBConnParams &params, uint8_t maxConn) {
    m_params = params;
    m_maxConn = maxConn;
    m_loopBreak = false;
    m_thread = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------//

PgConnPool::~PgConnPool() {
    m_loopBreak = true;
    if (m_thread) {
        m_thread->join();
        delete m_thread;
    }

    for (auto connInfo : m_connections) {
        delete connInfo;
    }
    m_connections.clear();
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgConnPool::Init() {
    //Trying to check connection to Postgres
    auto connInfo = createConnection();
    if (connInfo == nullptr) {
        return false;
    }

    //if all ok then adding opened connection to pool
    m_connections.push_back(connInfo);

//    m_thread = new std::thread(&PgConnPool::threadFunction, this);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------//

PgConnection *PgConnPool::GetConnection() {
    std::unique_lock<std::mutex> lk(m_connMutex);
    do {
        //1. Trying to search free already established connection
        for (auto connInfo : m_connections) {
            if (!connInfo->isBusy) {
                connInfo->isBusy = true;
                return connInfo->connection;
            }
        }
        //2.If not and connection count below max allowable, then try create new
        if (m_connections.size()<m_maxConn) {
            auto connInfo = createConnection();
            //if creating failed, then return nullptr as error indication
            if (connInfo->connection == nullptr) {
                return nullptr;
            }
            m_connections.push_back(connInfo);
            connInfo->isBusy = true;
            return connInfo->connection;
        }
        //3. If no free connections, then wait for one available
        cv.wait(lk);
    } while(true);

    return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------//

void PgConnPool::ReturnConnection(PgConnection *connection) {
    m_connMutex.lock();
    for (auto connInfo : m_connections) {
        if (connInfo->connection == connection) {
            connInfo->isBusy = false;
            if (!connInfo->connection->CheckConnection()) {
                removeConnection(connInfo);
            }
            break;
        }
    }
    m_connMutex.unlock();
    cv.notify_one();
}

//--------------------------------------------------------------------------------------------------------------------//

PgConnPool::SConnInfo *PgConnPool::createConnection() {
    auto conn = new PgConnection(m_params);
    if (!conn->Open()) {
        delete conn;
        return nullptr;
    }

    auto connInfo = new SConnInfo;
    connInfo->connection = conn;
    return connInfo;
}

//--------------------------------------------------------------------------------------------------------------------//

void PgConnPool::removeConnection(PgConnPool::SConnInfo *connInfo) {
    m_connections.remove(connInfo);
    delete connInfo;
}

//--------------------------------------------------------------------------------------------------------------------//

void PgConnPool::threadFunction() {
    do {
        if (m_loopBreak) break;


        std::this_thread::yield();
    } while(true);
}
