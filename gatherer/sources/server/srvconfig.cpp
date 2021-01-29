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

    bool res = true;
    if (document.HasMember("db")) {
        res = parseDBObject(document["db"]);
    }


    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

const char hostParam[] = "host";
const char portParam[] = "port";
const char userParam[] = "user";
const char passwordParam[] = "password";
const char dbNameParam[] = "dbname";

bool SrvConfig::parseDBObject(rapidjson::Value &object) {
    DBConnParams connParams{};
    if (object.HasMember(hostParam)) {
        if (!object[hostParam].IsString()) {
            printf("Invalid value for db.host.");
            return false;
        }
        connParams.host = object[hostParam].GetString();
    }
    if (object.HasMember(portParam)) {
        if (!object[portParam].IsUint()) {
            printf("Invalid value for db.port.");
            return false;
        }
        connParams.port = object[portParam].GetUint();
    }
    if (object.HasMember(userParam)) {
        if (!object[userParam].IsString()) {
            printf("Invalid value for db.user.");
            return false;
        }
        connParams.user = object[userParam].GetString();
    }
    if (object.HasMember(passwordParam)) {
        if (!object[passwordParam].IsString()) {
            printf("Invalid value for db.password.");
            return false;
        }
        connParams.password = object[passwordParam].GetString();
    }
    if (object.HasMember(dbNameParam)) {
        if (!object[dbNameParam].IsString()) {
            printf("Invalid value for db.dbname.");
            return false;
        }
        connParams.dbName = object[dbNameParam].GetString();
    } else {
        printf("DB name not defined.");
        return false;
    }

    if (connParams.user.empty()) {
        if (!connParams.password.empty()) {
            printf("Password defined w/o user.");
            return false;
        }
    }

    m_dbParams = connParams;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------//

DBConnParams SrvConfig::GetDBParams() {
    return m_dbParams;
}

//--------------------------------------------------------------------------------------------------------------------//

void SrvConfig::printHelp() {
    printf("The Orb gatherer.\n");
    printf("Service for gathering data from EDDN.\n");
    printf("Usage:\n");
    printf("gatherer -c path_to_config_file [-h]\n");
}
