#include "Resources.h"

std::string Resources::LoadTextFile(std::string path)
{
	std::ifstream myInput(RESOURCES_PATH + path);
	if (!myInput.good())
	{
		Debug::Error("File failed to load " + path);
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(myInput),
		std::istreambuf_iterator<char>()
	);

}
