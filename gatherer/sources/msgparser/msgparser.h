#ifndef MSGPARSER_H
#define MSGPARSER_H

#include "common/concurrentqueue.h"
#include "common/spaceobjects.h"

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>

/// Class for
class MsgParser {
public:
    MsgParser();
    ~MsgParser();
    ///Function to start message parser loop
    void Start();
    ///Add message to incoming queue
    void AddMessageToQueue(const std::string &message);

    std::function<void(MarketData &marketData)> MarketDataReceived;
private:
    ///Flag to thread exit
    std::atomic<bool> m_threadBreak{};
    std::thread *m_thread;

    ConcurrentQueue<std::string> m_msgQueue;

    void threadFunction();

    void parseMessage(const std::string& message);
};


#endif //MSGPARSER_H
