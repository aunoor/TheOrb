#include "psysparser.h"
#include "common/spaceobjects.h"

//--------------------------------------------------------------------------------------------------------------------//

bool PSysParser::ParseJson(const std::string &json) {
    rapidjson::Document document;
    document.Parse(json.c_str());

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

    if (!object.HasMember("id")) {
        return false;
    }
    if (!object.HasMember("id64")) {
        return false;
    }
    if (!object.HasMember("name")) {
        return false;
    }
    if (!object.HasMember("coords")) {
        return false;
    }



//    "id": 480,
//    "id64": 16063312045545,
//    "name": "Narasa",
//    "coords": {
//        "x": -50.78125,
//        "y": 22.59375,
//        "z": 168.3125
//    },

    uint32_t id = object["id"].GetInt();
    uint64_t id64 = object["id64"].GetInt64();
    std::string name(object["name"].GetString());


    StarSystem starSystem(id, id64, name, {});

    return false;
}
