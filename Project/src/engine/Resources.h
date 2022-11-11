#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "../api/debugModule.h"
#include "../api/objects/models.h"
#include "../api/renders/texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define RESOURCES_PATH "resources/"

// A centrialized static class to load files with determined relative path
class Resources
{
public:
	static std::string LoadTextFile(std::string path);
	static Mesh* LoadObject(std::string path);
	static ImageData* LoadImageData(std::string path);
	static std::vector<ImageData*> LoadCubemap(std::vector<std::string> paths);
	static void FreeImage(ImageData* image);

private:

};