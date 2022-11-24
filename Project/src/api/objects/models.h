#pragma once
#include <vector>
#include <map>
#include <fstream>
#include <glm/glm.hpp>
#include "../renderModule.h"

// struct for storing the obj file
struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

class Model {
public:
	Model();
	Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Texture* texture);
	~Model();

	void SetData(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void SetTexture(Texture* texture);
	void SetNormalMap(Texture* texture);
	void Draw(Shader* shader);

private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	Texture* m_texture, * m_normalMap;

	VAO m_vao;
	VBO m_vbo;
	EBO m_ebo;
};