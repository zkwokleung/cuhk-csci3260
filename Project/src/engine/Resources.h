#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "../api/debugModule.h"

#define RESOURCES_PATH "resources/"

// A centrialized static class to load files with determined relative path
class Resources
{
public:
	static std::string LoadTextFile(std::string path);

private:

};