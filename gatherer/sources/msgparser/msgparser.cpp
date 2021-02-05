#include "msgparser.h"

#include "eddndefs.h"
#include "commodityparser.h"
#include "logger/slogger.h"

#include <rapidjson/document.h>

//--------------------------------------------------------------------------------------------------------------------//

MsgParser::MsgParser() {
    m_thread = nullptr;
    m_threadBreak = false;
}

//--------------------------------------------------------------------------------------------------------------------//

MsgParser::~MsgParser() {
    if (m_thread) {
        m_threadBreak = true;
        m_thread->join();
        delete m_thread;
    }
}

//--------------------------------------------------------------------------------------------------------------------//

void MsgParser::Start() {
    m_thread = new std::thread(&MsgParser::threadFunction, this);
}

//--------------------------------------------------------------------------------------------------------------------//

void MsgParser::AddMessageToQueue(const std::string &message) {
    m_msgQueue.AddItem(message);
}

//--------------------------------------------------------------------------------------------------------------------//

void MsgParser::parseMessage(const std::string &message) {
    rapidjson::Document document;

    SLogger::Dump("EDDN", message);

    document.Parse(message.c_str());

    if (!document.HasMember(schemaRefName)) {
        SLogger::Warning("MsgParser", "Message don't have schemaref field");
        return;
    }

    if (!document.HasMember(messageName)) {
        SLogger::Warning("MsgParser", "Message don't have message field");
        return;
    }

    std::string schemaRef = document[schemaRefName].GetString();
    do {
        if (schemaRef == "https://eddn.edcd.io/schemas/journal/1") {
            break;
        }
        if (schemaRef == "https://eddn.edcd.io/schemas/commodity/3") {

            if (MarketDataReceived) {
                MarketData marketData;
                if (CommodityParser::Parse(document["message"], marketData)) {
                    SLogger::Info("MsgParser", asprintf("Received commodity data for %s->%s",marketData.SystemName.c_str(), marketData.StationName.c_str()));
                    MarketDataReceived(marketData);
                } else {
                    SLogger::Warning("MsgParser", "Failed to parse Commodity message.");
                }
            }
            break;
        }
    } while(false);
}

//--------------------------------------------------------------------------------------------------------------------//

void MsgParser::threadFunction() {
    std::string msg;
    //main loop
    do {
        //checking for exit flag
        if (m_threadBreak) break;
        msg = m_msgQueue.GetItem();
        parseMessage(msg);
    } while (true);
}
