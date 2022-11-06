#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "../api/debugModule.h"
#include "../api/objects/models.h"
#include "../api/renders/texture.h"

#define RESOURCES_PATH "resources/"

// A centrialized static class to load files with determined relative path
class Resources
{
public:
	static std::string LoadTextFile(std::string path);
	static Model LoadObject(std::string path);
	static ImageData* LoadImage(std::string path);
	static std::vector<ImageData*> LoadCubemap(std::vector<std::string> paths);
	static void FreeImage(ImageData* image);

private:

};