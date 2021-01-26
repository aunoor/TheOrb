#ifndef PSYSPARSER_H
#define PSYSPARSER_H

#include "common/spaceobjects.h"
#include <functional>
#include <string>
#include <rapidjson/document.h>

class PSysParser {
public:
    bool ParseJson(const std::string &json);
    std::function<void(StarSystem &system)> SystemReceived;
private:
    bool parseSystemObject(rapidjson::Value &object);
};


#endif //PSYSPARSER_H
