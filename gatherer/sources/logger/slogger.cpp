#include "slogger.h"

//--------------------------------------------------------------------------------------------------------------------//

static SLogger *m_instance = nullptr;

//--------------------------------------------------------------------------------------------------------------------//

const std::string LDumpStr("Dump");
/// Идентификатор уровня вывода сообщений 'debug'
const std::string LDebugStr("Debug");
/// Идентификатор уровня вывода сообщений 'info'
const std::string LInfoStr("Info");
/// Идентификатор уровня вывода сообщений 'warning'
const std::string LWarningStr("Warning");
/// Идентификатор уровня вывода сообщений 'critical'
const std::string LCriticalStr("Critical");
//--------------------------------------------------------------------------------------------------------------------//

std::string EType2String(ESLogLevel type) {
    switch(type) {
        case ESLogLevel::LL_Dump:
        case ESLogLevel::LL_Debug:
            return LDebugStr;
        case ESLogLevel::LL_Info:
            return LInfoStr;
        case ESLogLevel::LL_Warning:
            return LWarningStr;
        case ESLogLevel::LL_Critical:
            return LCriticalStr;
        default:
            break;
    }
    return std::string();
}

//--------------------------------------------------------------------------------------------------------------------//

void FallbackMessageLog(const std::string &msgLevel, const std::string &scope, const std::string &event) {
#ifdef _WIN32
    std::string str=msgLevel + " ["+scope+"]: "+event+"\n";
    printf("%s", str.c_str());
#endif
}

//--------------------------------------------------------------------------------------------------------------------//

SLogger *SLogger::GetInstance() {
    if (m_instance == nullptr) {
        m_instance = new SLogger();
    }
    return m_instance;
}

//--------------------------------------------------------------------------------------------------------------------//

SLogger::SLogger() {
    m_logLevel = LL_Warning;
}

//--------------------------------------------------------------------------------------------------------------------//

void SLogger::SetLogLevel(ESLogLevel level) {
    m_logLevel = level;
}

//--------------------------------------------------------------------------------------------------------------------//

void SLogger::LogMessage(ESLogLevel msgType, const std::string &scope, const std::string &event) {
    if (msgType > m_logLevel) return;

    if (msgType == LL_Dump) {
        return;
    }


    FallbackMessageLog(EType2String(msgType), scope, event);
}

//--------------------------------------------------------------------------------------------------------------------//

void SLogger::Debug(const std::string &scope, const std::string &event) {
    SLogger::GetInstance()->LogMessage(LL_Debug, scope, event);
}

void SLogger::Info(const std::string &scope, const std::string &event) {
    SLogger::GetInstance()->LogMessage(LL_Info, scope, event);
}

void SLogger::Warning(const std::string &scope, const std::string &event) {
    SLogger::GetInstance()->LogMessage(LL_Warning, scope, event);
}

void SLogger::Critical(const std::string &scope, const std::string &event) {
    SLogger::GetInstance()->LogMessage(LL_Critical, scope, event);
}

void SLogger::Dump(const std::string &scope, const std::string &event) {
    SLogger::GetInstance()->LogMessage(LL_Dump, scope, event);
}
