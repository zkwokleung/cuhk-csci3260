#include "models.h"

Model::Model() : m_vertices(), m_indices(), m_texture(nullptr), m_vao(), m_vbo(), m_ebo()
{
}

Model::Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Texture* texture) :
	m_vertices(vertices), m_indices(indices), m_texture(texture), m_vao(),
	m_vbo(&m_vertices[0], vertices.size() * sizeof(Vertex)),
	m_ebo(&m_indices[0], indices.size())
{
	m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_vao.LinkAttrib(m_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	m_vao.LinkAttrib(m_vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}

Model::~Model()
{
	m_vao.Delete();
	m_ebo.Delete();
	m_vbo.Delete();
}

void Model::SetData(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	m_vertices = vertices;
	m_indices = indices;
	m_vbo = VBO(&m_vertices[0], vertices.size() * sizeof(Vertex));
	m_ebo = EBO(&m_indices[0], indices.size());
	m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_vao.LinkAttrib(m_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	m_vao.LinkAttrib(m_vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}

void Model::SetTexture(Texture* texture)
{
	m_texture = texture;
}

void Model::SetNormalMap(Texture* texture)
{
	m_normalMap = texture;
}

void Model::Draw(Shader* shader)
{
	// Draw Texture
	if (m_texture)
	{
		m_texture->Bind(0);
	}
	// Normal Map
	shader->SetInt("u_useNormalMap", m_normalMap != nullptr);
	if (m_normalMap != nullptr)
	{
		m_normalMap->Bind(1);
	}

	Renderer::Draw(m_vao, m_ebo);
}
