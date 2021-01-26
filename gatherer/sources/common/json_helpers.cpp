#include "json_helpers.h"

bool json2coords(rapidjson::Value &object, SStarCoords &coords) {
    if (!object.HasMember("x") || !object.HasMember("y") || !object.HasMember("z")) {
        return false;
    }

    coords.x = object["x"].GetFloat();
    coords.y = object["y"].GetFloat();
    coords.z = object["z"].GetFloat();

    return true;
}
