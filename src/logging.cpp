#include "Logging.h"

#include <functional>

const std::string logName = "DEBUG";

std::shared_ptr<spdlog::logger> logCustom() {
	return spdlog::get(logName);
}

void logDebug(const std::string& msg) {
	spdlog::get(logName)->debug(msg);
}

void logInfo(const std::string& msg) {
	spdlog::get(logName)->info(msg);
}

void logWarn(const std::string& msg) {
	spdlog::get(logName)->warn(msg);
}

void logErr(const std::string& msg) {
	spdlog::get(logName)->error(msg);
}

