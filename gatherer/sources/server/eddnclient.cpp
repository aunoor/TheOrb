#include "eddnclient.h"
#include "logger/slogger.h"

#include <zlib.h>
#include <zmq.h>

#include <chrono>
#include <string>
#include <utility>

//--------------------------------------------------------------------------------------------------------------------//

const char scope[] = "EDDNClient";

//--------------------------------------------------------------------------------------------------------------------//

EDDNClient::EDDNClient(std::string eddnAddress) {
    m_thread = nullptr;
    m_threadBreak = false;
    m_eddnAddress = std::move(eddnAddress);
}

//--------------------------------------------------------------------------------------------------------------------//

EDDNClient::~EDDNClient() {
    if (m_thread) {
        m_threadBreak = true;
        m_thread->join();
        delete m_thread;
    }
}

//--------------------------------------------------------------------------------------------------------------------//

void EDDNClient::Start() {
    m_thread = new std::thread(&EDDNClient::threadFunction, this);
}

//--------------------------------------------------------------------------------------------------------------------//

#define BUFFER_LEN 3*1024*1024 //3 MB
void EDDNClient::threadFunction() {
    char *uncBuffer = (char*)malloc(BUFFER_LEN); //buffer for uncompressed data

    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_SUB);

    int zmqRes = -1;

    bool isConnected = false;

    //main loop
    do {
        //checking for exit flag
        if (m_threadBreak) break;

        if (!isConnected) {
            //start trying connect to EDDN
            SLogger::Info(scope, "Trying connect to EDDN stream...");
            do {
                zmqRes = zmq_connect(socket, m_eddnAddress.c_str());
                if (zmqRes != 0) {
                    SLogger::Warning(scope, "Connectiong to EDDN failed.");
                    break;
                }
                zmqRes = zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "", 0);
                if (zmqRes != 0) {
                    SLogger::Warning(scope, "Failing set ZMQ socket params.");
                    zmq_close(socket);
                    break;
                }
                isConnected = true;
            } while (false);

            if (!isConnected) {
                SLogger::Warning(scope, "Connect to EDDN failed.");
                //wait for 5 sec before reconnect
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                continue;
            } else {
                SLogger::Info(scope, "Connect to EDDN successful.");
            }
        }//if (!isConnected)

        zmq_msg_t msg;
        zmqRes = zmq_msg_init(&msg);
        if (zmqRes != 0) {
            SLogger::Critical(scope, "Fail to init ZMQ msg.");
            break;
        }

        do {
            //receive messages in blocking mode
            zmqRes = zmq_msg_recv(&msg, socket, 0);
            if (zmqRes < 0) {
                SLogger::Critical(scope, "Fail to receive ZMQ msg.");
                zmq_close(context);
                isConnected = false;
                break;
            }

            if (zmqRes) {
                uLongf destSize = BUFFER_LEN;
                int res = uncompress((Bytef *) uncBuffer, &destSize, (Bytef *) zmq_msg_data(&msg), zmqRes);

                if (res == Z_OK) {
                    std::string resStr(uncBuffer, destSize);
                    if (MessageReceived) {
                        MessageReceived(resStr);
                    }
                } else {
                    SLogger::Critical(scope, "Fail to inflate EDDN msg.");
                }
            }
        } while(false);

        zmq_msg_close(&msg);
    } while(true);

    free(uncBuffer);

    zmq_close(socket);
    zmq_ctx_destroy(context);
}