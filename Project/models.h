#pragma once
#include "objects.h"
#include "texture.h"
#include "shader.h"
#include "material.h"
#include <vector>
#include <map>
#include <fstream>

// struct for storing the obj file
struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Model {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	static Model loadOBJ(const char* objPath);
};

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