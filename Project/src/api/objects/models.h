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

struct Model {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};