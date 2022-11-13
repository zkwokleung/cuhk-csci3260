#pragma once
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <glew/glew.h>
#include "../renderModule.h"

class Skybox
{
public:
	Skybox();
	Skybox(GLuint64 handle, Shader* shader);
	~Skybox();

	void Enable();
	void Disable();

	void SetCubemap(GLuint64 handle);
	void SetShader(std::string vertexShader, std::string fragmentShader);

	static void Init(void);
	static void Draw();
private:
	GLuint64 m_cubemap;
	Shader* m_shader;

	static const float s_skyboxVertices[];
	static Skybox* s_activeSkybox;
	static VAO* s_vao;
	static VBO* s_vbo;
	static bool s_initialized;
};