#pragma once
#include "object.h"

class VerticesObject : public Object
{
public:
	VerticesObject();
	VerticesObject(const GLfloat vertices[], int elementCount);
	~VerticesObject();

	virtual void SetVertices(const GLfloat vertices[], int elementCount);
	virtual void OnPaint(Shader* shader);

protected:
	VAO* m_vao;
	VBO* m_vbo;
	int m_elementCount;
};