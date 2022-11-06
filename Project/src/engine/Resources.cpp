#include "Resources.h"

std::string Resources::LoadTextFile(std::string path)
{
	std::ifstream myInput(RESOURCES_PATH + path);
	if (!myInput.good())
	{
		std::stringstream msg;
		msg << "File failed to load " << RESOURCES_PATH << path;
		Debug::Error(msg.str());
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(myInput),
		std::istreambuf_iterator<char>()
	);

}
