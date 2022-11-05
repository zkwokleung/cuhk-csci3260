#pragma once
#include "light.h"
#include <sstream>

// Point Light
class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quaratic);
	~PointLight();

	virtual void OnPaint(Shader* shader);
	virtual std::string GetUniformNamePrefix() const;
	virtual void SetActive(bool active);

	void SetIntensity(float intensity);
	float GetIntensity() const;

private:
	int m_id;
	float m_constant, m_linear, m_quadratic, m_intensity;

	static int s_activeCount;
};