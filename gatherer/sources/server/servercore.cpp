#include "servercore.h"
#include "eddnclient.h"
#include "srvconfig.h"
#include "msgparser/msgparser.h"
#include "dbmanager/dbmanager.h"
#include "dbmanager/memprovider.h"
#include "dbmanager/postgres/pgprovider.h"
#include "edsmfetcher/psysfetcher.h"
#include "edsmfetcher/psysparser.h"

//--------------------------------------------------------------------------------------------------------------------//

ServerCore::ServerCore(SrvConfig *config) {
    m_canExit = false;
    m_eddnClient = nullptr;
    m_msgParser = nullptr;
    m_config = config;
}

//--------------------------------------------------------------------------------------------------------------------//

ServerCore::~ServerCore() {
    delete m_eddnClient;
}

//--------------------------------------------------------------------------------------------------------------------//

bool ServerCore::CanExit() {
    return m_canExit;
}

//--------------------------------------------------------------------------------------------------------------------//

bool ServerCore::Init() {
    //m_dbManager = new DBManager(new MemProvider());
    m_dbManager = new DBManager(new PgProvider(m_config->GetDBParams()));
    bool res = m_dbManager->Init();
    if (!res) {
        return false;
    }

    m_eddnClient = new EDDNClient("tcp://eddn.edcd.io:9500");
    m_msgParser = new MsgParser();
    m_msgParser->MarketDataReceived = [this](MarketData &marketData) {
        m_dbManager->StoreMarketData(marketData);
    };
    m_eddnClient->MessageReceived = std::bind(&MsgParser::AddMessageToQueue, m_msgParser, std::placeholders::_1);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------//

void ServerCore::Start() {
    bool res = m_dbManager->IsSystemsLoaded();
    if (!res) {
        PSysFetcher pSysFetcher;
        pSysFetcher.DownloadingProgress = [](int32_t current, int32_t length) {
            if (length > 0) {
                uint32_t progress = ((uint64_t)current * 100L) / length;
                printf("Downloaded %u from %u (%u%%)\n", current, length, progress);
            }
        };
        uint64_t sysCnt = 0;
        pSysFetcher.SystemReceived = [this, &sysCnt](StarSystem &system) {
            m_dbManager->UpdateSystem(system);
            sysCnt++;
        };

        res = pSysFetcher.FetchPopulatedSystems();
        if (!res) {
            m_canExit = true;
            return;
        }

        printf("Downloading finished. %d systems added\n", sysCnt);
    }
    //m_canExit = true;


    m_msgParser->Start();
    m_eddnClient->Start();
}

//--------------------------------------------------------------------------------------------------------------------//
