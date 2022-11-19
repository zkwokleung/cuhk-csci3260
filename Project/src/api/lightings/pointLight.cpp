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
	shader->SetFloat(GetUniformNamePrefix() + ".constant", m_constant);
	shader->SetFloat(GetUniformNamePrefix() + ".linear", m_linear);
	shader->SetFloat(GetUniformNamePrefix() + ".quadratic", m_quadratic);

	shader->SetVec3(GetUniformNamePrefix() + ".position", GetTransform().GetPosition());

	shader->SetFloat(GetUniformNamePrefix() + ".intensity", m_intensity);

	shader->SetInt(GetUniformNamePrefix() + ".isActive", 1);
}

std::string PointLight::GetUniformNamePrefix() const
{
	std::stringstream ss;
	ss << "pointLights[" << std::to_string(m_id) << "]";
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

void PointLight::SetPointLightParams(float constant, float linear, float quaratic)
{
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quaratic;
}

void PointLight::SetConstant(float constant)
{
	m_constant = constant;
}

void PointLight::SetLinear(float linear)
{
	m_linear = linear;
}

void PointLight::SetQuaratic(float quaratic)
{
	m_quadratic = quaratic;
}

float PointLight::GetIntensity() const
{
	return m_intensity;
}

int PointLight::s_activeCount = 0;