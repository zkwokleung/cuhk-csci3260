#include "spotLight.h"

SpotLight::SpotLight() : PointLight(), m_direction(glm::vec3(.0f, -1.f, .0f)), m_cutOff(12.5f), m_innerCutOff(0.99f)
{

}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	float constant, float linear, float quaratic,
	glm::vec3 direction, float cutOff, float innerCutOff) :
	PointLight(ambient, diffuse, specular, constant, linear, quaratic),
	m_direction(direction), m_cutOff(cutOff), m_innerCutOff(innerCutOff)
{
}

SpotLight::~SpotLight()
{
}

std::string SpotLight::GetUniformNamePrefix() const
{
	return "spotLight";
}

void SpotLight::OnPaint(Shader* shader)
{
	PointLight::OnPaint(shader);
	shader->setVec3(GetUniformNamePrefix() + ".position", GetTransform().GetPosition());
	shader->setVec3(GetUniformNamePrefix() + ".direction", m_direction);
	shader->setFloat(GetUniformNamePrefix() + ".cutOff", m_cutOff);
	shader->setFloat(GetUniformNamePrefix() + ".innerCutOff", m_innerCutOff);
}

void SpotLight::SetDirection(glm::vec3 direction)
{
	m_direction = direction;
}

glm::vec3 SpotLight::GetDirection() const
{
	return m_direction;
}

void SpotLight::SetCutOff(float cutOff)
{
	m_cutOff = cutOff;
}

float SpotLight::GetCutOff() const
{
	return m_cutOff;
}

void SpotLight::SetInnerCutOff(float innerCutOff)
{
	m_innerCutOff = innerCutOff;
}

float SpotLight::GetInnerCutOff() const
{
	return m_innerCutOff;
}