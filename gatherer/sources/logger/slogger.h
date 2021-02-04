#ifndef SLOGGER_H
#define SLOGGER_H

#include "common/commondefs.h"

class SLogger {
public:
    SLogger();
    /// Set log level
    void SetLogLevel(ESLogLevel level);

    void LogMessage(ESLogLevel msgType, const std::string &scope, const std::string &event);

    void Debug(const std::string &scope, const std::string &event);
    void Info(const std::string &scope, const std::string &event);
    void Warning(const std::string &scope, const std::string &event);
    void Critical(const std::string &scope, const std::string &event);
private:
    ///Current log level
    ESLogLevel m_logLevel;
};


#endif //SLOGGER_H
