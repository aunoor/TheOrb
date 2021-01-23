#include "msgparser.h"

#include "common/schemadefs.h"
#include "commodityparser.h"

#include <rapidjson/document.h>
#include <iostream>

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

void MsgParser::AddMessageToQueue(std::string message) {
    m_msgQueue.AddEvent(message);
}

//--------------------------------------------------------------------------------------------------------------------//

void MsgParser::parseMessage(const std::string &message) {
    rapidjson::Document document;
    document.Parse(message.c_str());

    if (!document.HasMember(schemaRefName.c_str())) {
        //TODO:log
        return;
    }

    std::string schemaRef = document[schemaRefName.c_str()].GetString();

    if (schemaRef == "https://eddn.edcd.io/schemas/journal/1") {
        ;
    } else if (schemaRef == "https://eddn.edcd.io/schemas/commodity/3") {
        std::cout << schemaRef << std::endl;
        CommodityParser::Parse(document);

    }

}

//--------------------------------------------------------------------------------------------------------------------//

void MsgParser::threadFunction() {
    std::string msg;
    //main loop
    do {
        //checking for exit flag
        if (m_threadBreak) break;
        msg = m_msgQueue.WaitEvent();
        parseMessage(msg);
    } while (true);
}
