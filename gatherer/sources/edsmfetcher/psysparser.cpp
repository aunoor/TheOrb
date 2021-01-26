#include "psysparser.h"
#include "common/json_helpers.h"
#include "common/schemadefs.h"

#include "rapidjson/reader.h"
#include <iostream>
#include <sstream>
//--------------------------------------------------------------------------------------------------------------------//

bool parseStations(rapidjson::Value &object, StarSystem &starSystem);

//--------------------------------------------------------------------------------------------------------------------//

struct MyHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, MyHandler> {
    bool Null() { std::cout << "Null()" << std::endl; return true; }
    bool Bool(bool b) { std::cout << "Bool(" << std::boolalpha << b << ")" << std::endl; return true; }
    bool Int(int i) { std::cout << "Int(" << i << ")" << std::endl; return true; }
    bool Uint(unsigned u) { std::cout << "Uint(" << u << ")" << std::endl; return true; }
    bool Int64(int64_t i) { std::cout << "Int64(" << i << ")" << std::endl; return true; }
    bool Uint64(uint64_t u) { std::cout << "Uint64(" << u << ")" << std::endl; return true; }
    bool Double(double d) { std::cout << "Double(" << d << ")" << std::endl; return true; }
    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        std::cout << "String(" << str << ", " << length << ", " << std::boolalpha << copy << ")" << std::endl;
        return true;
    }
    bool StartObject() { std::cout << "StartObject()" << std::endl; return true; }
    bool Key(const char* str, rapidjson::SizeType length, bool copy) {
        std::cout << "Key(" << str << ", " << length << ", " << std::boolalpha << copy << ")" << std::endl;
        return true;
    }
    bool EndObject(rapidjson::SizeType memberCount) { std::cout << "EndObject(" << memberCount << ")" << std::endl; return true; }
    bool StartArray() { std::cout << "StartArray()" << std::endl; return true; }
    bool EndArray(rapidjson::SizeType elementCount) { std::cout << "EndArray(" << elementCount << ")" << std::endl; return true; }
};

//--------------------------------------------------------------------------------------------------------------------//

using namespace rapidjson;

bool PSysParser::ParseJson(const std::string &json) {
    rapidjson::Document document;
    document.Parse(json.c_str());

    StringStream stream(json.c_str());
    MyHandler handler;
    rapidjson::Reader reader;

    reader.Parse<kParseIterativeFlag>(stream, handler);

    return false;

    auto array = document.GetArray();
    if (array.Empty()) {
        return false;
    }

    for (auto& v : array) {
        rapidjson::Value a = v.GetObject();
        parseSystemObject(a);
    }

    return false;
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

    if (object.HasMember(stationsName)) {
        res = parseStations(object[stationsName], starSystem);
        if (!res) {
            return false;
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

    if (!object.HasMember(idName)) {
        return false;
    }
    station.Id = object[idName].GetInt();

    if (!object.HasMember(marketIdName)) {
        return false;
    }
    station.MarketId = object[marketIdName].GetInt();

    if (!object.HasMember(nameName)) {
        return false;
    }
    station.Name = object[nameName].GetString();

    if (!object.HasMember(typeName)) {
        return false;
    }
    station.Type = object[typeName].GetString();

    if (!object.HasMember(dist2ArrivalName)) {
        return false;
    }
    station.Dist2Arrival = object[dist2ArrivalName].GetFloat();

    if (!object.HasMember(haveMarketName)) {
        return false;
    }
    station.Dist2Arrival = object[haveMarketName].GetFloat();


    starSystem.Stations.push_back(station);
    return true;
}
