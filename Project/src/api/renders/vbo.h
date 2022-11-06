#pragma once
#include <glew/glew.h>
#include <freeglut/glut.h>
#include <freeglut/freeglut.h>
#include <freeglut/freeglut_std.h>
#include <glm/glm.hpp>

class VBO
{
public:
	GLuint ID;
	VBO();
	VBO(const GLfloat* vertices, GLsizeiptr size);
	~VBO();

	// Bind the VBO to allow OpenGL to use it
	void Bind() const;
	void Unbind() const;
	void Delete();
};