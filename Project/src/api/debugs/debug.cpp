#include "Debug.h"

void Debug::Init(void)
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLMessageCallback, 0);
}

void Debug::Log(std::string message)
{
	std::cout << message << std::endl;
}

void Debug::LogWarning(std::string message)
{
	std::cout << "\033[33m" << "Warning: " << message << "\033[0m" << std::endl;
}

void Debug::LogError(std::string message)
{
	std::cout << "\033[1;31m" << "!!" << message << "!!" << "\033[0m" << std::endl;
}

void Debug::LogOpenGLError(void)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{

	}
}

void Debug::GLMessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		std::stringstream msg;
		msg << "OpenGL Error: " << message;
		LogError(msg.str());
	}
}