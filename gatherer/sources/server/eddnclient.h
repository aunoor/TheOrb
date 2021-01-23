#ifndef EDDNCLIENT_H
#define EDDNCLIENT_H

#include <atomic>
#include <functional>
#include <thread>

///Class for gathering EDDN data
class EDDNClient {
public:
    explicit EDDNClient(std::string eddnAddress);

    ~EDDNClient();
    ///Function to start gathering EDDN messages
    void Start();
    ///Callback called when new message received
    std::function<void(std::string message)> MessageReceived = nullptr;

private:
    ///Flag to thread exit
    std::atomic<bool> m_threadBreak;
    std::thread *m_thread;
    std::string m_eddnAddress;

    void threadFunction();
};


#endif //EDDNCLIENT_H
