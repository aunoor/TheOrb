#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <atomic>

class EDDNClient;
class MsgParser;
class DBManager;

class ServerCore {
public:
    ServerCore();
    ~ServerCore();
    ///Function return if program can break main loop
    bool CanExit();
    ///Function for service initialization
    bool Init();
    ///Function for service start
    void Start();

private:
    ///Flag for breaking main loop
    std::atomic<bool> m_canExit;

    EDDNClient *m_eddnClient;
    MsgParser *m_msgParser;
    DBManager *m_dbManager;
};


#endif //SERVERCORE_H
