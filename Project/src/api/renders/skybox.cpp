#include "skybox.h"
#include "../cameras/camera.h"

Skybox::Skybox() : m_cubemap(0), m_shader(new Shader())
{
}

Skybox::Skybox(GLuint64 handler, Shader* shader) : m_cubemap(handler), m_shader(shader)
{
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

void Skybox::SetCubemap(GLuint64 handle)
{
	m_cubemap = handle;
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
	s_vbo = new VBO(Skybox::s_skyboxVertices, 36 * 8 * sizeof(float));
	s_vao->LinkAttrib(*s_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);
}

void Skybox::Draw()
{
	if (s_activeSkybox == nullptr || Camera::GetMain() == nullptr)
		return;

	glDepthMask(GL_FALSE);
	s_activeSkybox->m_shader->Use();
	// Set view and projection
	s_activeSkybox->m_shader->SetMat4("u_projectionMatrix", Camera::GetMain()->GetProjectionMatrix());
	s_activeSkybox->m_shader->SetMat4("u_viewRotateMatrix", Camera::GetMain()->GetViewRotationMatrix());

	// Render skybox
	s_vao->Bind();
	s_activeSkybox->m_shader->SetHandleui64ARB("cubemap", s_activeSkybox->m_cubemap);
	glDrawArrays(GL_TRIANGLES, 0, 48);

	// Enable depth mask again
	glDepthMask(GL_TRUE);
}

const float Skybox::s_skyboxVertices[] = {
	// z negative (front)
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	// x negative (left)
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	// x positive (right)
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	 // z positive (back)
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	// y positive (top)
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	// y negative (bottom)
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};