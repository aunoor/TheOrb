#include "server/servercore.h"
#include "server/srvconfig.h"
#include "logger/slogger.h"

#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{
    SLogger::GetInstance()->SetLogLevel(LL_Debug);

    SrvConfig config;
    if (!config.ParseConfig(argc, argv)) {
        return -1;
    }

    ServerCore serverCore(&config);

    if (!serverCore.Init()) {
        return -2;
    }

    serverCore.Start();

    while(true) {
        if (serverCore.CanExit()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::this_thread::yield();
    }
    return 0;
}
