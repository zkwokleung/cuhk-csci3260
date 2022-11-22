#pragma once
#include "object.h"

#define DEFAULT_VERTIX_COLOR 0.8f, 0.8f, 0.8f, 1.0f

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