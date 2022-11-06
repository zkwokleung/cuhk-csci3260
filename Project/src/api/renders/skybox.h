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
	Skybox(std::vector<ImageData*> images, Shader* shader);
	~Skybox();

	void Enable();
	void Disable();

	void SetCubemap(std::vector<ImageData*> images);
	void SetShader(std::string vertexShader, std::string fragmentShader);

	static void Init(void);
	static void Draw();
private:
	unsigned int m_cubemap;
	Shader* m_shader;

	static const float s_skyboxVertices[];
	static Skybox* s_activeSkybox;
	static VAO* s_vao;
	static VBO* s_vbo;
	static bool s_initialized;
};