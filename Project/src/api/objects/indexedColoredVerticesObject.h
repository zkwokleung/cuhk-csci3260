#pragma once
#include "../renderModule.h"
#include "transform.h"
#include "coloredVerticesObject.h"

class IndexedColoredVerticesObject : public ColoredVerticesObject
{
public:
	IndexedColoredVerticesObject();
	IndexedColoredVerticesObject(const GLfloat vertices[], int elementCount, const GLuint indices[], int indicesCount);
	~IndexedColoredVerticesObject();

	virtual void SetIndices(const GLuint indices[], int indicesCount);
	virtual void OnPaint(Shader* shader);

protected:
	EBO* m_ebo;
	int m_indicesCount;
};