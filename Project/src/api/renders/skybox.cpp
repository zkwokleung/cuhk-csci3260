#include "skybox.h"
#include "../cameras/camera.h"

Skybox::Skybox() : m_cubemap(0), m_shader(new Shader())
{
}

Skybox::Skybox(std::vector<ImageData*> images, Shader* shader) : m_shader(shader)
{
	SetCubemap(images);
}

Skybox::~Skybox()
{

}

void Skybox::Enable()
{
	s_activeSkybox = this;
}

void Skybox::Disable()
{
	s_activeSkybox = nullptr;
}

void Skybox::SetCubemap(std::vector<ImageData*> images)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < images.size(); i++)
	{
		if (images[i]->data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, images[i]->Width, images[i]->Height, 0, GL_RGB, GL_UNSIGNED_BYTE, images[i]->data
			);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::SetShader(std::string vertexShader, std::string fragmentShader)
{
	m_shader->SetupShader(vertexShader, fragmentShader);
}

// Static
Skybox* Skybox::s_activeSkybox = nullptr;

VAO* Skybox::s_vao = nullptr;
VBO* Skybox::s_vbo = nullptr;

void Skybox::Init(void)
{
	s_vao = new VAO();
	s_vbo = new VBO(Skybox::s_skyboxVertices, 36 * sizeof(float));
	s_vao->LinkAttrib(*s_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);
}

void Skybox::Draw()
{
	if (s_activeSkybox == nullptr)
		return;

	glDepthMask(GL_FALSE);
	s_activeSkybox->m_shader->Use();
	// Set view and projection
	Camera::OnPaint(s_activeSkybox->m_shader);

	// Render skybox
	s_vao->Bind();
	glBindTexture(GL_TEXTURE_CUBE_MAP, s_activeSkybox->m_cubemap);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Enable depth mask again
	glDepthMask(GL_TRUE);
}

const float Skybox::s_skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};