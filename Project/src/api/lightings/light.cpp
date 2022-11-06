#include "light.h"

Light::Light() : Object(), m_ambient(glm::vec3()), m_diffuse(glm::vec3()), m_specular(glm::vec3())
{

}

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Object(),
m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
{
}

Light::~Light()
{

}

void Light::SetAmbient(glm::vec3 ambient)
{
	m_ambient = ambient;
}

void Light::SetDiffuse(glm::vec3 diffuse)
{
	m_diffuse = diffuse;
}

void Light::SetSpecular(glm::vec3 specular)
{
	m_specular = specular;
}

glm::mat4 Light::GetProjectionMat4()
{
	return glm::mat4();
}

glm::mat4 Light::GetViewMat4()
{
	return glm::mat4();
}

void Light::OnPaint(Shader* shader)
{
	shader->SetVec3(GetUniformNamePrefix() + ".ambient", m_ambient);
	shader->SetVec3(GetUniformNamePrefix() + ".diffuse", m_diffuse);
	shader->SetVec3(GetUniformNamePrefix() + ".specular", m_specular);
}

std::string Light::GetUniformNamePrefix() const
{
	return "light";
}