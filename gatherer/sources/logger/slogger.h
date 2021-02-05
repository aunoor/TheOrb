#ifndef SLOGGER_H
#define SLOGGER_H

#include "common/commondefs.h"

class SLogger {
public:
    static SLogger *GetInstance();

    /// Set log level
    void SetLogLevel(ESLogLevel level);

    void LogMessage(ESLogLevel msgType, const std::string &scope, const std::string &event);

    static void Debug(const std::string &scope, const std::string &event);
    static void Info(const std::string &scope, const std::string &event);
    static void Warning(const std::string &scope, const std::string &event);
    static void Critical(const std::string &scope, const std::string &event);
    static void Dump(const std::string &scope, const std::string &event);
private:
    SLogger();
    ///Current log level
    ESLogLevel m_logLevel;
};


#endif //SLOGGER_H
