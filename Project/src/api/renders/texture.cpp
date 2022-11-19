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
	data = image->data;
}

void Texture::Bind(unsigned int slot = 0) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, data);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
