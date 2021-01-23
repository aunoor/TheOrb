#ifndef MSG_COMMON_H
#define MSG_COMMON_H

#include <cstdint>

enum MsgType : uint8_t{
    mtUnknown = 0,
    mtCommodity = 1,
};

class AbstractMsg {
public:
    ///Return type of message
    virtual MsgType GetType() = 0;

};

#endif //MSG_COMMON_H
