#include "material.h"

Material::Material() : m_ambient(glm::vec3(1.f)), m_diffuse(1.f), m_specular(1.f), m_shininess(.5f)
{

}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
	m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
{

}

Material::~Material()
{
}

void Material::OnPaint(Shader* shader)
{
	shader->SetVec3("material.ambient", m_ambient);
	shader->SetVec3("material.diffuse", m_diffuse);
	shader->SetVec3("material.specular", m_specular);
	shader->SetFloat("material.shininess", m_shininess);
}

void Material::SetAmbient(glm::vec3 ambient)
{
	m_ambient = ambient;
}

void Material::SetDiffuse(glm::vec3 diffuse)
{
	m_diffuse = diffuse;
}

void Material::SetSpecular(glm::vec3 specular)
{
	m_specular = specular;
}

void Material::SetShininess(float shininess)
{
	m_shininess = shininess;
}