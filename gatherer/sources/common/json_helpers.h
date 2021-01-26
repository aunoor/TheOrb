#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <rapidjson/document.h>
#include "spaceobjects.h"

//Convert json object to coords struct
bool json2coords(rapidjson::Value &object, SStarCoords &coords);

#endif //JSON_HELPERS_H
