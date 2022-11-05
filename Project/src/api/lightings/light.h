#pragma once
#include <sstream>
#include "../objects/object.h"
#include "../renders/shader.h"

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