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

void Texture::Draw(Shader* shader)
{
	shader->SetHandleui64ARB("u_texture", data);
}

void Texture::Bind(unsigned int slot = 0) const
{

}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
