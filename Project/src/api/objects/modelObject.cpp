#include "modelObject.h"

ModelObject::ModelObject() : Object(), m_mesh()
{

}

ModelObject::ModelObject(Mesh* model) : Object(),
m_mesh(model), m_material()
{
}

ModelObject::ModelObject(Mesh* mesh, Material material) : Object(),
m_mesh(mesh), m_material(material)
{
}

ModelObject::~ModelObject()
{
	delete m_mesh;
}

Mesh* ModelObject::GetMesh()
{
	return m_mesh;
}

void ModelObject::SetMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

Material& ModelObject::GetMaterial()
{
	return m_material;
}

void ModelObject::OnPaint(Shader* shader)
{
	Object::OnPaint(shader);
	m_material.OnPaint(shader);
	m_mesh->Draw(shader);
}