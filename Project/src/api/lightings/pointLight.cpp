#include "pointLight.h"

PointLight::PointLight() : Light(), m_constant(.0f), m_linear(.0f), m_quadratic(.0f), m_intensity(1.f), m_id(-1)
{

}
PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quaratic) :
	Light(ambient, diffuse, specular), m_constant(constant), m_linear(linear), m_quadratic(quaratic),
	m_intensity(1.f), m_id(-1)
{
}

PointLight::~PointLight()
{
}

void PointLight::OnPaint(Shader* shader)
{
	Light::OnPaint(shader);
	shader->setFloat(GetUniformNamePrefix() + ".constant", m_constant);
	shader->setFloat(GetUniformNamePrefix() + ".linear", m_linear);
	shader->setFloat(GetUniformNamePrefix() + ".quadratic", m_quadratic);

	shader->setVec3(GetUniformNamePrefix() + ".position", GetTransform().GetPosition());

	shader->setFloat(GetUniformNamePrefix() + ".intensity", m_intensity);

	shader->setInt(GetUniformNamePrefix() + ".isActive", 1);
}

std::string PointLight::GetUniformNamePrefix() const
{
	std::stringstream ss;
	char id = m_id + '0';
	ss << "pointLights[" << id << "]";
	return ss.str();
}

void PointLight::SetActive(bool active)
{
	if (active)
	{
		if (!IsActive())
		{
			m_id = s_activeCount++;
		}
	}
	else
	{
		if (IsActive())
		{
			m_id = -1;
			s_activeCount--;
		}
	}

	Object::SetActive(active);
}

void PointLight::SetIntensity(float intensity)
{
	m_intensity = intensity;
}

float PointLight::GetIntensity() const
{
	return m_intensity;
}

int PointLight::s_activeCount = 0;