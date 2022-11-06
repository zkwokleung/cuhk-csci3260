#pragma once
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <glew/glew.h>
#include <stb_image/stb_image.h>
#include "../renderModule.h"

class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> faces, const char* vertexPath, const char* fragmentPath);
	~Skybox();

	void Enable();
	void Disable();

	void LoadCubemap(std::vector<std::string> faces);
	void LoadShader(std::string vertexShader, std::string fragmentShader);

	static void Init(void);
	static void Draw();
	static unsigned int loadCubemap(std::vector<std::string> faces);
private:
	unsigned int m_cubemap;
	Shader* m_shader;

	static const float s_skyboxVertices[];
	static Skybox* s_activeSkybox;
	static VAO* s_vao;
	static VBO* s_vbo;
	static bool s_initialized;
};