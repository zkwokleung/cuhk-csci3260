#pragma once
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>

class Shader {
public:
	Shader();
	Shader(std::string vertexShader, std::string fragmentShader);
	~Shader();

	void SetupShader(std::string vertexShader, std::string fragmentShader);
	void Use() const;

	// a series utilities for setting shader parameters 
	void SetMat4(const std::string& name, glm::mat4& value) const;
	void SetVec4(const std::string& name, glm::vec4 value) const;
	void SetVec3(const std::string& name, glm::vec3 value) const;
	void SetVec3(const std::string& name, float v1, float v2, float v3) const;
	void SetFloat(const std::string& name, float value) const;
	void SetInt(const std::string& name, int value) const;

private:
	unsigned int ID = 0;

	bool CheckShaderStatus(GLuint shaderID) const;
	bool CheckProgramStatus(GLuint programID) const;
	bool CheckStatus(
		GLuint objectID,
		PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
		PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
		GLenum statusType) const;
};
