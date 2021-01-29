#ifndef COMMONDEFS_H
#define COMMONDEFS_H

#include <cstdint>
#include <string>

struct DBConnParams {
    std::string host;
    int32_t port=-1;
    std::string user;
    std::string password;
    std::string dbName;
};

#endif //COMMONDEFS_H
