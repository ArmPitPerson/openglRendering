#include "files.h"
#include "logging.h"

#include <sstream>
#include <fstream>


const std::string readFile(const std::string& filepath) {
	std::stringstream out;
	std::ifstream file(filepath);
	if (file.is_open()) {
		out << file.rdbuf();
		return out.str();
	}
	logCustom()->warn("Failed to read file: {}", filepath);
	return std::string("");
}
