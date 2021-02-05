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

enum ESLogLevel : uint8_t {
    /// Критические сообщения
    LL_Critical = 0,
    /// Предупреждения
    LL_Warning = 1,
    /// Информационные сообщения
    LL_Info = 2,
    /// Отладочные сообщения
    LL_Debug = 3,
    /// Дамп пришедших данных
    LL_Dump = 4
};

std::string asprintf(const char *format, ...);


#endif //COMMONDEFS_H
