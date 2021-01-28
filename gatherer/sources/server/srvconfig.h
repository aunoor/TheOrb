#ifndef SRVCONFIG_H
#define SRVCONFIG_H

#include <string>

class SrvConfig {
public:
    SrvConfig();
    bool ParseConfig(int argc, char **argv);
private:
    std::string m_cfgFileName;

    void printHelp();

    bool parseConfigFile();
};


#endif //SRVCONFIG_H
