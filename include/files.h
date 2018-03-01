///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef FILES_H
#define FILES_H

#include <string>

//************************************
// Method:    readFile
// Access:    public 
// Parameter: const std::string & filepath
// Brief:     Return entire file contents as a string
//************************************
const std::string readFile(const std::string& filepath);

//************************************
// Method:    getResourcePath
// Access:    public 
// Parameter: const std::string & resourceName
// Brief:     Get the absolute path to the given resource located in /res/'resourceName'
// Example:   loadResource(getResourcePath("Texture.png"));
//************************************
std::string getResourcePath(const std::string& resourceName);

#endif // FILES_H
