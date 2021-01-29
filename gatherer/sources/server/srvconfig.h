#ifndef SRVCONFIG_H
#define SRVCONFIG_H

#include "common/commondefs.h"
#include <rapidjson/document.h>
#include <string>

class SrvConfig {
public:
    SrvConfig();
    bool ParseConfig(int argc, char **argv);
    ///Return connection params
    DBConnParams GetDBParams();

private:
    std::string m_cfgFileName;
    DBConnParams m_dbParams{};

    void printHelp();
    bool parseConfigFile();
    bool parseDBObject(rapidjson::Value &object);
};


#endif //SRVCONFIG_H
