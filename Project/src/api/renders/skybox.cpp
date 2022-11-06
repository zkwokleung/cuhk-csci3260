#include "skybox.h"
#include "../cameras/camera.h"

Skybox::Skybox() : m_cubemap(0), m_shader(new Shader())
{
}

Skybox::Skybox(std::vector<std::string> faces, const char* vertexPath, const char* fragmentPath) : m_shader(new Shader(vertexPath, fragmentPath))
{
	m_cubemap = loadCubemap(faces);
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

void Skybox::LoadCubemap(std::vector<std::string> faces)
{
	m_cubemap = loadCubemap(faces);
}

void Skybox::LoadShader(const char* vertexPath, const char* fragmentPath)
{
	m_shader->setupShader(vertexPath, fragmentPath);
}

// Static
Skybox* Skybox::s_activeSkybox = nullptr;

VAO* Skybox::s_vao = nullptr;
VBO* Skybox::s_vbo = nullptr;

void Skybox::Init(void)
{
	s_vao = new VAO();
	s_vbo = new VBO(Skybox::s_skyboxVertices, 36);
	s_vao->LinkAttrib(*s_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);
}

void Skybox::Draw()
{
	if (s_activeSkybox == nullptr)
		return;

	glDepthMask(GL_FALSE);
	s_activeSkybox->m_shader->use();
	// Set view and projection
	Camera::OnPaint(s_activeSkybox->m_shader);

	// Render skybox
	s_vao->Bind();
	glBindTexture(GL_TEXTURE_CUBE_MAP, s_activeSkybox->m_cubemap);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Enable depth mask again
	glDepthMask(GL_TRUE);
}

unsigned int Skybox::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
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