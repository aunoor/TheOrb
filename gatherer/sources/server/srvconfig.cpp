#include "srvconfig.h"

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include <getopt.h>
#include <string>

//--------------------------------------------------------------------------------------------------------------------//

SrvConfig::SrvConfig() = default;

//--------------------------------------------------------------------------------------------------------------------//

bool SrvConfig::ParseConfig(int argc, char **argv) {
    std::string optString = "c:";
    int opt = getopt(argc, argv, optString.c_str());
    while( opt != -1 ) {
        switch( opt ) {
            case 'c':
                m_cfgFileName = optarg;
                break;
            case 'h':
            case '?':
                printHelp();
                return false;
            default:
                break;
        }
        opt = getopt(argc, argv, optString.c_str());
    }

    if (m_cfgFileName.empty()) {
        return false;
    }

    return parseConfigFile();
}

//--------------------------------------------------------------------------------------------------------------------//

bool SrvConfig::parseConfigFile() {
    FILE* fp = fopen(m_cfgFileName.c_str(), "rb");
    if (fp == nullptr) {
        printf("Can't open config file %s", m_cfgFileName.c_str());
        return false;
    }

    char readBuffer[65536];
    rapidjson::FileReadStream fs(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document document;
    document.ParseStream(fs);
    fclose(fp);

    if (document.IsNull()) {
        printf("Can't parse config file %s: ", m_cfgFileName.c_str());
        return false;
    }



    return true;
}

//--------------------------------------------------------------------------------------------------------------------//

void SrvConfig::printHelp() {
    printf("The Orb gatherer.\n");
    printf("Service for gathering data from EDDN.\n");
    printf("Usage:\n");
    printf("gatherer -c path_to_config_file [-h]\n");
}
