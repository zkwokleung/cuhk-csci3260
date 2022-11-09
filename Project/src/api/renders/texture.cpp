#include "texture.h"

Texture::Texture(ImageData* data)
{
	SetData(data);
}

Texture::~Texture()
{
}

void Texture::SetData(ImageData* image)
{
	BPP = image->BPP;
	Width = image->Width;
	Height = image->Height;

	GLenum format = 3;
	switch (BPP) {
	case 1: format = GL_RED; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	}

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (image->data) {
		glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, image->data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(unsigned int slot = 0) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
