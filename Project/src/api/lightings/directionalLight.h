#pragma once
#include "light.h"
#include <sstream>

// Directional Light
class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction, float intensity);
	~DirectionalLight();

	void SetDirection(glm::vec3 direction);
	glm::vec3 GetDirection() const;

	void SetIntensity(float intensity);
	float GetIntensity() const;

	virtual glm::mat4 GetProjectionMat4();
	virtual glm::mat4 GetViewMat4();

	virtual void OnPaint(Shader* shader);

private:
	glm::vec3 m_direction;
	float m_intensity;

	virtual std::string GetUniformNamePrefix() const;
};