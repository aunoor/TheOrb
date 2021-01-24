#include "server/servercore.h"

#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{
    ServerCore serverCore;

    if (!serverCore.Init()) {
        return -1;
    }

    serverCore.Start();

    while(true) {
        if (serverCore.CanExit()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::this_thread::yield();
    }
    return 0;
}
