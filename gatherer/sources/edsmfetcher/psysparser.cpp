#include "psysparser.h"
#include "common/json_helpers.h"
#include "common/schemadefs.h"

#include "rapidjson/reader.h"
#include <iostream>
#include <sstream>

//--------------------------------------------------------------------------------------------------------------------//

bool parseStations(rapidjson::Value &object, StarSystem &starSystem);

//--------------------------------------------------------------------------------------------------------------------//

class CharStream {
public:
    typedef char Ch; //Необходимый элемент для работы Reader

    CharStream() = default;

    char Peek() const {
        return m_dataQueue.PeekItem();
    }

    char Take() {
        return m_dataQueue.GetItem();
    }

    size_t Tell() const {
        return m_dataQueue.Size();
    }

    void Put(char data) {
        m_dataQueue.AddItem(data);
    }

    char* PutBegin() {
        //assert(false);
        m_dataQueue.StartAdding();
        return nullptr;
    }
    size_t PutEnd(char*) {
        //assert(false);
        m_dataQueue.StopAdding();
        return 0;
    }
    void Flush() { assert(false); }

    CharStream(const CharStream&) = delete;
    CharStream& operator=(const CharStream&) = delete;

private:
    mutable ConcurrentQueue<char> m_dataQueue;
};

//--------------------------------------------------------------------------------------------------------------------//

struct MyHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, MyHandler> {
    bool Null() {
        //std::cout << "Null()" << std::endl;
        return true;
    }
    bool Bool(bool b) {
        //std::cout << "Bool(" << std::boolalpha << b << ")" << std::endl;
        return true;
    }
    bool Int(int i) {
        //std::cout << "Int(" << i << ")" << std::endl;
        return true;
    }
    bool Uint(unsigned u) {
        //std::cout << "Uint(" << u << ")" << std::endl;
        return true;
    }
    bool Int64(int64_t i) {
        //std::cout << "Int64(" << i << ")" << std::endl;
        return true;
    }
    bool Uint64(uint64_t u) {
        //std::cout << "Uint64(" << u << ")" << std::endl;
        return true;
    }
    bool Double(double d) {
        //std::cout << "Double(" << d << ")" << std::endl;
        return true;
    }
    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        //std::cout << "String(" << str << ", " << length << ", " << std::boolalpha << copy << ")" << std::endl;
        return true;
    }
    bool StartObject() {
        //std::cout << "StartObject()" << std::endl;
        return true;
    }
    bool Key(const char* str, rapidjson::SizeType length, bool copy) {
        //std::cout << "Key(" << str << ", " << length << ", " << std::boolalpha << copy << ")" << std::endl;
        return true;
    }
    bool EndObject(rapidjson::SizeType memberCount) {
        //std::cout << "EndObject(" << memberCount << ")" << std::endl;
        return true;
    }
    bool StartArray() {
        //std::cout << "StartArray()" << std::endl;
        return true;
    }
    bool EndArray(rapidjson::SizeType elementCount) {
        //std::cout << "EndArray(" << elementCount << ")" << std::endl;
        return true;
    }
};

//--------------------------------------------------------------------------------------------------------------------//

PSysParser::PSysParser() {
    m_charStream = new CharStream();
    m_thread = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------//

PSysParser::~PSysParser() {
    if (m_thread) {
        m_thread->join();
        delete m_thread;
    }
    delete m_charStream;
}

//--------------------------------------------------------------------------------------------------------------------//

void PSysParser::AddData(char *data, size_t length) {
    m_charStream->PutBegin();
    for (int i = 0; i < length; i++) {
        m_charStream->Put(data[i]);
    }
    m_charStream->PutEnd(nullptr);
}

//--------------------------------------------------------------------------------------------------------------------//

void PSysParser::StartParse() {
    m_thread = new std::thread(&PSysParser::threadFunction, this);
}

//--------------------------------------------------------------------------------------------------------------------//

void PSysParser::threadFunction() {
    MyHandler handler;
    rapidjson::Reader reader;
    std::cout << "Start Parse" << std::endl;
    reader.Parse<rapidjson::kParseIterativeFlag>(*m_charStream, handler);
    std::cout << "End Parse" << std::endl;
}

//--------------------------------------------------------------------------------------------------------------------//


bool PSysParser::ParseJson(const std::string &json) {
    rapidjson::Document document;
    document.Parse(json.c_str());

    rapidjson::StringStream stream(json.c_str());
    MyHandler handler;
    rapidjson::Reader reader;
    reader.Parse<rapidjson::kParseIterativeFlag>(stream, handler);

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



