#include "lights.h"

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
	shader->setVec3(GetUniformNamePrefix() + ".ambient", m_ambient);
	shader->setVec3(GetUniformNamePrefix() + ".diffuse", m_diffuse);
	shader->setVec3(GetUniformNamePrefix() + ".specular", m_specular);
}

std::string Light::GetUniformNamePrefix() const
{
	return "light";
}

DirectionalLight::DirectionalLight() : Light(), m_direction(), m_intensity(1.f)
{

}
DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction, float intensity) :
	Light(ambient, diffuse, specular), m_direction(direction), m_intensity(intensity)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::SetDirection(glm::vec3 direction)
{
	m_direction = direction;
}

glm::vec3 DirectionalLight::GetDirection() const
{
	return m_direction;
}

void DirectionalLight::SetIntensity(float intensity)
{
	m_intensity = intensity;
}

float DirectionalLight::GetIntensity() const
{
	return m_intensity;
}

void DirectionalLight::OnPaint(Shader* shader)
{
	Light::OnPaint(shader);
	shader->setVec3(GetUniformNamePrefix() + ".direction", m_direction);
	shader->setFloat(GetUniformNamePrefix() + ".intensity", m_intensity);
}

glm::mat4 DirectionalLight::GetProjectionMat4()
{
	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	return lightProjection;
}

glm::mat4 DirectionalLight::GetViewMat4()
{
	return glm::lookAt(GetTransform().GetPosition(), GetDirection(), GetTransform().GetUp());
}

std::string DirectionalLight::GetUniformNamePrefix() const
{
	return "dirLight";
}

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