#pragma once
#include <glew/glew.h>
#include <iostream>

#include "../renders/shader.h"

struct ImageData
{
	GLuint data;
	int Width = 0, Height = 0, BPP = 0;
};

class Texture
{
public:
	Texture(ImageData* data);
	~Texture();

	void SetData(ImageData* image);
	void Bind(unsigned int slot) const;
	void Unbind() const;

private:
	GLuint data;
};