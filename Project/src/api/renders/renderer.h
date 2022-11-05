#pragma once
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "vao.h"
#include "ebo.h"

#define DEFAULT_CLEAR_COLOR 

// The Renderer is responsible for drawing things to the window.
class Renderer
{
public:
	// Clear everything that is currently displayed
	static void Clear();
	// Draw an element with the given vertices and the specific shape
	static void Draw(const VAO& vao, GLenum mode, int vertexCount);
	// Draw an element with the given vertices array and indices array
	static void Draw(const VAO& vao, const EBO& ebo);
	// Set the clear color
	static void SetClearColor(glm::vec4 clearColor);
private:
	static glm::vec4 s_clearColor;
};