#include "shader.h"

Shader::Shader()
{
}

Shader::Shader(std::string vertexShader, std::string fragmentShader)
{
	SetupShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
}

void Shader::SetupShader(std::string vertexShader, std::string fragmentShader)
{
	// similar to the installShaders() in the assignment 1
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* vCode;
	vCode = vertexShader.c_str();
	glShaderSource(vertexShaderID, 1, &vCode, NULL);

	const GLchar* fCode;
	fCode = fragmentShader.c_str();
	glShaderSource(fragmentShaderID, 1, &fCode, NULL);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!CheckShaderStatus(vertexShaderID) || !CheckShaderStatus(fragmentShaderID))
		return;

	ID = glCreateProgram();

	glAttachShader(ID, vertexShaderID);
	glAttachShader(ID, fragmentShaderID);
	glLinkProgram(ID);

	if (!CheckProgramStatus(ID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(0);
}


void Shader::Use() const
{
	glUseProgram(ID);
}

int Shader::SetMat4(const std::string& name, glm::mat4& value)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));

	return 0;
}

int Shader::SetVec4(const std::string& name, glm::vec4 value)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniform4fv(loc, 1, &value[0]);

	return 0;
}

int Shader::SetVec3(const std::string& name, glm::vec3 value)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniform3fv(loc, 1, &value[0]);

	return 0;
}

int Shader::SetVec3(const std::string& name, float v1, float v2, float v3)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniform3f(loc, v1, v2, v3);

	return 0;
}

int Shader::SetFloat(const std::string& name, float value)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniform1f(loc, value);

	return 0;
}

int Shader::SetInt(const std::string& name, int value)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniform1i(loc, value);

	return 0;
}

int Shader::SetHandleui64ARB(const std::string& name, GLuint64 value)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniformHandleui64ARB(loc, value);

	return 0;
}

// Private 

int Shader::GetUniformLocation(std::string name)
{
	// Try to find the location from cache
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[name];

	// Try to get the uniform location
	int loc = glGetUniformLocation(ID, name.c_str());

	// Check if the location exist
	if (loc == -1)
	{
		if (m_uniformWarnings.find(name) == m_uniformWarnings.end())
		{
			std::stringstream msg;
			msg << "Cannot find uniform location of " << name << " in " << __FILE__ << ":" << __LINE__;
			Debug::Warning(msg.str());
			// Record that the warning has been displayed
			m_uniformWarnings[name] = true;
		}

		return -1;
	}

	// Save to cache
	m_uniformLocationCache[name] = loc;

	return loc;
}


bool Shader::CheckShaderStatus(GLuint shaderID) const
{
	return CheckStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool Shader::CheckProgramStatus(GLuint programID) const
{
	return CheckStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

bool Shader::CheckStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType) const
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}