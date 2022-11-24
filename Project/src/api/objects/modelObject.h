#pragma once
#include "models.h"
#include "object.h"

class ModelObject : public Object
{
public:
	ModelObject();
	ModelObject(Model* mesh);
	ModelObject(Model* mesh, Material material);
	~ModelObject();

	Model* GetMesh();
	void SetMesh(Model* model);

	Material& GetMaterial();

	virtual void OnPaint(Shader* shader);

private:
	Model* m_mesh;
	Material m_material;
};