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
    ///Function return url for EDSM systemsPopulated json file.
    std::string GetSPURL();

private:
    std::string m_cfgFileName;
    DBConnParams m_dbParams{};
    std::string m_spURL;

    void printHelp();
    bool parseConfigFile();
    bool parseDBObject(rapidjson::Value &object);
};


#endif //SRVCONFIG_H
