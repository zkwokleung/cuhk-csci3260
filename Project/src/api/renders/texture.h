#pragma once
#include <glew/glew.h>
#include <stb_image/stb_image.h>

#include <iostream>

struct ImageData
{
	unsigned char* data;
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
	unsigned int ID = 0;
	int Width = 0, Height = 0, BPP = 0;
};