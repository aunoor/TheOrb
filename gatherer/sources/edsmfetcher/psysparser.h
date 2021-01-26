#ifndef PSYSPARSER_H
#define PSYSPARSER_H

#include "common/spaceobjects.h"
#include "common/concurrentqueue.h"
#include <rapidjson/document.h>
#include <functional>
#include <string>
#include <thread>

class CharStream;

class PSysParser {
public:
    PSysParser();
    ~PSysParser();
    void AddData(char *data, size_t length);
    bool ParseJson(const std::string &json);
    void StartParse();

    std::function<void(StarSystem &system)> SystemReceived;
private:

    std::thread *m_thread;

    CharStream *m_charStream;

    bool parseSystemObject(rapidjson::Value &object);


    void threadFunction();
};


#endif //PSYSPARSER_H
