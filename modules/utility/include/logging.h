/// GL_Rendering by Carl Findahl (C) 2017
/// A Kukon Development Project

#ifndef LOGGING_H
#define LOGGING_H

#include "spdlog/spdlog.h"

#include <functional>

namespace detail {
    const std::string logName = "DEBUG";
}

/* LOGGING FUNCTIONS*/

//************************************
// Method:    logInfo
// Parameter: const std::string & msg
// Brief:     Log the message at debug level
//************************************
template<typename... T>
void logDebug(const std::string& msg, T&&... fmtargs)
{
    spdlog::get(detail::logName)->debug(msg.c_str(), std::forward<T>(fmtargs)...);
}

//************************************
// Method:    logInfo
// Parameter: const std::string & msg
// Brief:     Log the message at info level
//************************************
template<typename... T>
void logInfo(const std::string& msg, T&&... fmtargs)
{
    spdlog::get(detail::logName)->info(msg.c_str(), std::forward<T>(fmtargs)...);
}

//************************************
// Method:    logInfo
// Parameter: const std::string & msg
// Brief:     Log the message at warning level
//************************************
template<typename... T>
void logWarn(const std::string& msg, T&&... fmtargs)
{
    spdlog::get(detail::logName)->warn(msg.c_str(), std::forward<T>(fmtargs)...);
}

//************************************
// Method:    logInfo
// Parameter: const std::string & msg
// Brief:     Log the message at error level
//************************************
template<typename... T>
void logErr(const std::string& msg, T&&... fmtargs)
{
    spdlog::get(detail::logName)->error(msg.c_str(), std::forward<T>(fmtargs)...);
}

#endif // LOGGING_H
