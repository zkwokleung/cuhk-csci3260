#pragma once
#include "pointLight.h"
#include <sstream>

class SpotLight : public PointLight
{
public:
	SpotLight();
	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quaratic,
		glm::vec3 direction, float cutOff, float innerCutOff);
	~SpotLight();

	virtual std::string GetUniformNamePrefix() const;
	virtual void OnPaint(Shader* shader);

	void SetDirection(glm::vec3 direction);
	glm::vec3 GetDirection() const;

	void SetCutOff(float cutOff);
	float GetCutOff() const;
	void SetInnerCutOff(float innerCutOff);
	float GetInnerCutOff() const;

private:
	glm::vec3 m_direction;
	float m_cutOff;
	float m_innerCutOff;
};