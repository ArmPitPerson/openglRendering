#include "files.h"
#include "logging.h"

#include <sstream>
#include <fstream>
#include <experimental/filesystem>

using namespace std::experimental; // For filesystem access

const std::string readFile(const std::string& filepath) {
    std::stringstream out;
    std::ifstream file(filepath);
    if (file.is_open()) {
        out << file.rdbuf();
        return out.str();
    }
    else
    {
        logWarn("Failed to read file: {}", filepath);
        return std::string("");
    }
}

std::string getResourcePath(const std::string& resourceName)
{
    // Return the path to the resource in an OS-Agnostic way
#ifndef NDEBUG
    auto p = filesystem::v1::current_path().append("res").append(resourceName).string();
    logInfo("Getting resource path: {}", p);
    return p;
#else
    return filesystem::v1::current_path().append("res").append(resourceName).string();
#endif
}
