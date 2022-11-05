#pragma once
#include "models.h"
#include "object.h"

class ModelObject : public Object
{
public:
	ModelObject();
	ModelObject(const char* modelPath, const char* texturePath);
	ModelObject(Model model);
	ModelObject(Model model, Texture* texture);
	~ModelObject();

	void LoadModel(const char* path);
	void LoadTexture(const char* path);
	void SetTexture(Texture* texture);

	Material& GetMaterial();

	virtual void OnPaint(Shader* shader);

private:
	Model m_model;
	Texture* m_texture;
	Material m_material;
	VAO m_vao;
	VBO m_vbo;
	EBO m_ebo;
};