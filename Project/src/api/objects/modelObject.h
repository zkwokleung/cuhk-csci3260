#pragma once
#include "models.h"
#include "object.h"

class ModelObject : public Object
{
public:
	ModelObject();
	ModelObject(Mesh* mesh);
	ModelObject(Mesh* mesh, Material material);
	~ModelObject();

	Mesh* GetMesh();
	void SetMesh(Mesh* model);

	Material& GetMaterial();

	virtual void OnPaint(Shader* shader);

private:
	Mesh* m_mesh;
	Material m_material;
};