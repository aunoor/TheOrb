#include "psysparser.h"
#include "common/json_helpers.h"
#include "common/schemadefs.h"
#include <cctype>


//--------------------------------------------------------------------------------------------------------------------//

bool parseStations(rapidjson::Value &object, StarSystem &starSystem);

//--------------------------------------------------------------------------------------------------------------------//

PSysParser::PSysParser() {
    m_bracesCount = 0;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PSysParser::AddData(const char *data, size_t length) {

    if (m_state == epsEndOfArray) return true;

    for (int i = 0; i < length; i++) {
        char c = data[i];

        if (m_state == epsWaitStartArray || m_state == epsWaitObjectStart) {
            //пропускаем пробельные символы в ожидании начала объекта/массива
            if (isspace(c)) continue;
            if (c==',') continue;
        }

        switch (m_state) {
            case epsWaitStartArray:
                if (c == '[') {
                    m_state = epsWaitObjectStart;
                } else {
                    printf("Can't find array opening brace\n");
                    return false;
                }
                break;
            case epsWaitObjectStart:
                if (c == ']') {
                    m_state = epsEndOfArray;
                    break;
                }
                if (c == '{') {
                    m_state = epsWaitObjectEnd;
                    m_objectStr = c;
                    m_bracesCount = 1;
                } else {
                    printf("Can't find object opening brace\n");
                    return false;
                }
                break;
            case epsWaitObjectEnd:
                m_objectStr += c;
                if (c=='{') {
                    m_bracesCount++;
                }
                if (c=='}') {
                    m_bracesCount--;
                }
                if (m_bracesCount == 0) {
                    m_state = epsWaitObjectStart;
                    //printf("%s\n",m_objectStr.c_str());
                    if (!parseSystemObject(m_objectStr)) {
                        return false;
                    }
                }
                break;
            default:
                return false;
        }
    }

    return true;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PSysParser::parseSystemObject(std::string &object) {
    rapidjson::Document document;
    document.Parse(object.c_str());
    if (document.IsNull()) {
        printf("Error while parse sysobject\n");
    }
    bool res = parseSystemObject(document);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

void PSysParser::StartParse() {
    m_state = epsWaitStartArray;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PSysParser::parseSystemObject(rapidjson::Value &object) {
//    "id": 480,
//    "id64": 16063312045545,
//    "name": "Narasa",
//    "coords": {
//        "x": -50.78125,
//        "y": 22.59375,
//        "z": 168.3125
//    },

    StarSystem starSystem;

    if (!object.HasMember(idName)) {
        return false;
    }
    starSystem.Id = object[idName].GetInt();

    if (!object.HasMember(id64Name)) {
        return false;
    }
    starSystem.Id64 = object[id64Name].GetInt64();

    if (!object.HasMember(nameName)) {
        return false;
    }
    starSystem.Name=object[nameName].GetString();

    if (!object.HasMember(coordsName)) {
        return false;
    }
    SStarCoords coords{};
    bool res = json2coords(object[coordsName], coords);
    if (!res) {
        //TODO:log
        return false;
    }
    starSystem.Coords = coords;
    starSystem.Coords.IsValid = true;

    if (object.HasMember(stationsName)) {
        auto stations = object[stationsName].GetArray();
        for (auto& s : stations) {
            res = parseStations(s, starSystem);
            if (!res) {
                return false;
            }
        }
    }

    if (SystemReceived) {
        SystemReceived(starSystem);
    }
    return true;
}

//--------------------------------------------------------------------------------------------------------------------//

bool parseStations(rapidjson::Value &object, StarSystem &starSystem) {
/*
      "id": 38927,
      "marketId": 3231025920,
      "type": "Outpost",
      "name": "Fancher Enterprise",
      "distanceToArrival": 19.925188,
      "haveMarket": true,
 */

    Station station;
    //id
    if (!object.HasMember(idName) || object[idName].IsNull()) {
        return false;
    }
    station.Id = object[idName].GetInt();

    //marketId
    if (object.HasMember(marketIdName)) {
        if (!object[marketIdName].IsNull()) {
            station.MarketId = object[marketIdName].GetInt64();
        }
    }

    //name
    if (!object.HasMember(nameName)) {
        return false;
    }
    if (!object[nameName].IsNull()) {
        station.Name = object[nameName].GetString();
    }

    //type
    if (!object.HasMember(typeName)) {
        return false;
    }
    if (!object[typeName].IsNull()) {
        station.Type = object[typeName].GetString();
    }

    //dist2Arrival
    if (!object.HasMember(dist2ArrivalName)) {
        return false;
    }
    if (!object[dist2ArrivalName].IsNull()) {
        station.Dist2Arrival = object[dist2ArrivalName].GetFloat();
    }

    //haveMarket
    if (object.HasMember(haveMarketName)) {
        if (!object[haveMarketName].IsNull()) {
            station.HaveMarket = object[haveMarketName].GetBool();
        }
    }

    starSystem.Stations.push_back(station);
    return true;
}



