#include "directionalLight.h"

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