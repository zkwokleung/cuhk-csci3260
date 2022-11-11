#include "models.h"

Mesh::Mesh(): m_vertices(),m_indices(),m_texture(nullptr), m_vao(), m_vbo(), m_ebo()
{
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Texture* texture) :
	m_vertices(vertices), m_indices(indices), m_texture(texture), m_vao(),
	m_vbo(&m_vertices[0], vertices.size() * sizeof(Vertex)),
	m_ebo(&m_indices[0], indices.size())
{
	m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_vao.LinkAttrib(m_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	m_vao.LinkAttrib(m_vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}

Mesh::~Mesh()
{
	m_vao.Delete();
	m_ebo.Delete();
	m_vbo.Delete();
}

void Mesh::SetData(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	m_vertices = vertices;
	m_indices = indices;
}

void Mesh::SetTexture(Texture* texture)
{
	m_texture = texture;
}

void Mesh::Draw(Shader* shader)
{
	if (m_texture)
	{
		m_texture->Bind(0);
	}
	Renderer::Draw(m_vao, m_ebo);
}
