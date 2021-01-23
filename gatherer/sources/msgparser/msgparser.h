#ifndef MSGPARSER_H
#define MSGPARSER_H

#include "msgqueue.h"

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
    void AddMessageToQueue(std::string message);
private:
    ///Flag to thread exit
    std::atomic<bool> m_threadBreak{};
    std::thread *m_thread;

    MsgQueue m_msgQueue;

    void threadFunction();

    void parseMessage(const std::string& message);
};


#endif //MSGPARSER_H
