#pragma once
#include "objects.h"
#include <sstream>

// Base Light
class Light : public Object
{
public:
	Light();
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~Light();

	void SetAmbient(glm::vec3 ambient);
	void SetDiffuse(glm::vec3 diffuse);
	void SetSpecular(glm::vec3 specular);

	virtual glm::mat4 GetProjectionMat4();
	virtual glm::mat4 GetViewMat4();

	virtual void OnPaint(Shader* shader);

protected:
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	virtual std::string GetUniformNamePrefix() const;
};

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

// Spot Light
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