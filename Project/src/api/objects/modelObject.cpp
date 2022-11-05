#include "modelObject.h"

ModelObject::ModelObject() : Object(), m_model(), m_vao(), m_vbo(), m_ebo(), m_texture()
{

}

ModelObject::ModelObject(const char* modelPath, const char* texturePath) : Object(),
m_model(Model::loadOBJ(modelPath)), m_texture(new Texture()), m_material(),
m_vao(), m_vbo((GLfloat*)&(m_model.vertices[0]), m_model.vertices.size() * sizeof(Vertex)),
m_ebo((GLuint*)&m_model.indices[0], m_model.indices.size())
{
	m_texture->setupTexture(texturePath);

	m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	m_vao.LinkAttrib(m_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	m_vao.LinkAttrib(m_vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}

ModelObject::ModelObject(Model model) : Object(),
m_model(model), m_texture(nullptr), m_material(),
m_vao(), m_vbo((GLfloat*)&(m_model.vertices[0]), m_model.vertices.size() * sizeof(Vertex)),
m_ebo((GLuint*)&m_model.indices[0], m_model.indices.size())
{
	m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	m_vao.LinkAttrib(m_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	m_vao.LinkAttrib(m_vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}

ModelObject::ModelObject(Model model, Texture* texture) : Object(),
m_model(model), m_texture(texture), m_material(),
m_vao(), m_vbo((GLfloat*)&(m_model.vertices[0]), m_model.vertices.size() * sizeof(Vertex)),
m_ebo((GLuint*)&m_model.indices[0], m_model.indices.size())
{
	m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	m_vao.LinkAttrib(m_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	m_vao.LinkAttrib(m_vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}

ModelObject::~ModelObject()
{
	m_vao.Delete();
	m_vbo.Delete();
	m_ebo.Delete();
	//delete m_texture;
}

void ModelObject::LoadModel(const char* path)
{
	m_model = Model::loadOBJ(path);
	m_vbo = VBO((GLfloat*)&(m_model.vertices[0]), m_model.vertices.size() * sizeof(Vertex));
	m_ebo = EBO((GLuint*)&m_model.indices[0], m_model.indices.size());
}

void ModelObject::LoadTexture(const char* path)
{
	m_texture->setupTexture(path);
}

void ModelObject::SetTexture(Texture* texture)
{
	m_texture = texture;
}

Material& ModelObject::GetMaterial()
{
	return m_material;
}

void ModelObject::OnPaint(Shader* shader)
{
	Object::OnPaint(shader);
	m_texture->bind(0);
	m_material.OnPaint(shader);
	Renderer::Draw(m_vao, m_ebo);
}