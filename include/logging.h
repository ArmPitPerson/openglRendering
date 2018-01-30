/// GL_Rendering by Carl Findahl (C) 2017
/// A Kukon Development Project

#ifndef LOGGING_H
#define LOGGING_H

#include "spdlog/spdlog.h"

#include <functional>

/* LOGGING FUNCTIONS*/

//************************************
// Method:    logCustom
// Access:    public 
// Brief:     Get the underlying debug logger for custom and formatted logging
// Example:   logCustom()->info("There are {0} men alive", menAlive)
//************************************
std::shared_ptr<spdlog::logger> logCustom();

//************************************
// Method:    logInfo
// Parameter: const std::string & msg
// Brief:     Log the message at debug level
//************************************
void logDebug(const std::string& msg);

//************************************
// Method:    logInfo
// Parameter: const std::string & msg
// Brief:     Log the message at info level
//************************************
void logInfo(const std::string& msg);

//************************************
// Method:    logInfo
// Parameter: const std::string & msg
// Brief:     Log the message at warning level
//************************************
void logWarn(const std::string& msg);

//************************************
// Method:    logInfo
// Parameter: const std::string & msg
// Brief:     Log the message at error level
//************************************
void logErr(const std::string& msg);

#endif // LOGGING_H
