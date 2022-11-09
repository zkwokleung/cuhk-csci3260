#pragma once
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include "../debugModule.h"

class Shader {
public:
	Shader();
	Shader(std::string vertexShader, std::string fragmentShader);
	~Shader();

	void SetupShader(std::string vertexShader, std::string fragmentShader);
	void Use() const;

	// a series utilities for setting shader parameters 
	int SetMat4(const std::string& name, glm::mat4& value);
	int SetVec4(const std::string& name, glm::vec4 value);
	int SetVec3(const std::string& name, glm::vec3 value);
	int SetVec3(const std::string& name, float v1, float v2, float v3);
	int SetFloat(const std::string& name, float value);
	int SetInt(const std::string& name, int value);

private:
	unsigned int ID = 0;
	std::unordered_map<std::string, int> m_uniformLocationCache;
	std::unordered_map<std::string, bool> m_uniformWarnings; // Whether the warning has been displayed

	int GetUniformLocation(std::string name);
	bool CheckShaderStatus(GLuint shaderID) const;
	bool CheckProgramStatus(GLuint programID) const;
	bool CheckStatus(
		GLuint objectID,
		PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
		PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
		GLenum statusType) const;
};
