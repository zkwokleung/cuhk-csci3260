#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <glew/glew.h>

class Debug
{
public:
	static void Init(void);
	static void Log(std::string message);
	static void LogWarning(std::string message);
	static void LogError(std::string message);

	static void LogOpenGLError(void);

private:
	static void GLMessageCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam);
};