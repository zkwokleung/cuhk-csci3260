#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "../api/debugModule.h"
#include "../api/objects/models.h"

#define RESOURCES_PATH "resources/"

// A centrialized static class to load files with determined relative path
class Resources
{
public:
	static std::string LoadTextFile(std::string path);
	static Model LoadObject(std::string path);
	static unsigned char* LoadImage(std::string path);

private:

};