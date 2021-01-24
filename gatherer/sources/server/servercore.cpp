#include "servercore.h"
#include "eddnclient.h"
#include "msgparser/msgparser.h"
#include "dbmanager/dbmanager.h"
#include "dbmanager/memprovider.h"
#include "edsmfetcher/psysfetcher.h"

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
    m_dbManager = new DBManager(new MemProvider());

    m_eddnClient = new EDDNClient("tcp://eddn.edcd.io:9500");
    m_msgParser = new MsgParser();
    m_eddnClient->MessageReceived = std::bind(&MsgParser::AddMessageToQueue, m_msgParser, std::placeholders::_1);
    return true;
}

void ServerCore::Start() {
    bool res = m_dbManager->IsSystemsLoaded();
    if (!res) {
        PSysFetcher pSysFetcher;
        res = pSysFetcher.FetchPopulatedSystems();
        if (!res) {
            m_canExit = true;
            return;
        }
        std::string json;
        pSysFetcher.GetJSON(json);
    }
    m_canExit = true;



//    m_msgParser->Start();
//    m_eddnClient->Start();
}
