#include "Debug.h"

void Debug::Log(std::string message)
{
	std::cout << message << std::endl;
}

void Debug::Warning(std::string message)
{
	std::cout << "\033[33m" << "Warning: " << message << "\033[0m" << std::endl;
}

void Debug::Error(std::string message)
{
	std::cout << "\033[1;31m" << "!!" << message << "!!" << "\033[0m" << std::endl;
}