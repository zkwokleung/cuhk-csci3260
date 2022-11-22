#include "verticesObject.h"

VerticesObject::VerticesObject() : Object(), m_vao(new VAO()), m_vbo(nullptr), m_elementCount(0)
{

}

VerticesObject::VerticesObject(const GLfloat vertices[], int elementCount) : Object(), m_vao(new VAO())
{
	SetVertices(vertices, elementCount);
}

VerticesObject::~VerticesObject()
{
	m_vao->Delete();
	m_vbo->Delete();

	delete m_vao;
	delete m_vbo;
}

void VerticesObject::SetVertices(const GLfloat vertices[], int elementCount)
{
	m_vbo = new VBO(vertices, elementCount * sizeof(float));
	m_vao->LinkAttrib(*m_vbo, 0, 3, GL_FLOAT, 0, 0);

	m_elementCount = elementCount;
}

void VerticesObject::OnPaint(Shader* shader)
{
	Object::OnPaint(shader);
	glm::vec4 c = glm::vec4(DEFAULT_VERTIX_COLOR);
	shader->SetVec4("u_VertColor", c);
	Renderer::Draw(*m_vao, GL_TRIANGLES, m_elementCount / 3);
}