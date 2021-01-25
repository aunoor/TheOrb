#ifndef PSYSPARSER_H
#define PSYSPARSER_H

#include <functional>
#include <string>
#include "rapidjson/document.h"

class PSysParser {
public:
    bool ParseJson(const std::string &json);

private:
    bool parseSystemObject(rapidjson::Value &object);
};


#endif //PSYSPARSER_H
