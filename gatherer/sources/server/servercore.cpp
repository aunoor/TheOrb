#include "servercore.h"
#include "eddnclient.h"
#include "msgparser/msgparser.h"

ServerCore::ServerCore() {
    m_canExit = false;
    m_eddnClient = nullptr;
    m_msgParser = nullptr;
}

ServerCore::~ServerCore() {
    delete m_eddnClient;
}

bool ServerCore::CanExit() {
    return m_canExit;
}

bool ServerCore::Init() {
    m_eddnClient = new EDDNClient("tcp://eddn.edcd.io:9500");
    m_msgParser = new MsgParser();
    m_eddnClient->MessageReceived = std::bind(&MsgParser::AddMessageToQueue, m_msgParser, std::placeholders::_1);
    return true;
}

void ServerCore::Start() {
    m_msgParser->Start();
    m_eddnClient->Start();
}
