#ifndef PSYSPARSER_H
#define PSYSPARSER_H

#include "common/spaceobjects.h"
#include "common/concurrentqueue.h"
#include "common/commondefs.h"

#include <rapidjson/document.h>
#include <functional>
#include <string>
#include <thread>

enum EParseState {
    epsUnknown = 0,
    epsWaitStartArray = 1, //ожидание начального массива
    epsWaitObjectStart, //ожидание начала объекта системы
    epsWaitObjectEnd, //ожидание окончания описания объекта системы
    epsEndOfArray //найдена закрывающая скобка массива объектов систем
};

class PSysParser {
public:
    PSysParser();
    ///Add data to object extractor
    bool AddData(const char *data, size_t length);
    ///Reset extractor state
    void StartParse();

    std::function<void(StarSystem &system)> SystemReceived;
    std::function<void(ESLogLevel msgType, const std::string &event)> LogMsg;

private:
    ///Current state of object extractor
    EParseState m_state = epsUnknown;
    ///Accumulated string for object description
    std::string m_objectStr;
    ///Count of braces
    uint32_t m_bracesCount;

    bool parseSystemObject(std::string &object);
    bool parseSystemObject(rapidjson::Value &object);
    void logMsg(ESLogLevel msgType, const std::string &event);
};


#endif //PSYSPARSER_H
