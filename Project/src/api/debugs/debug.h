#pragma once
#include <string>
#include <iostream>

class Debug
{
public:
	static void Log(std::string message);
	static void Warning(std::string message);
	static void Error(std::string message);
};