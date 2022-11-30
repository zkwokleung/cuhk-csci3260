#include "mountain.h"

Model* Mountain::s_mesh = nullptr;
Texture* Mountain::s_texture = nullptr;

Mountain::Mountain()
{
	// Initialize the static model and texture if havent
	if (s_texture == nullptr)
	{
		s_texture = new Texture(Resources::LoadImageData("texture/rock-mountain.png"));
	}
	if (s_mesh == nullptr)
	{
		s_mesh = Resources::LoadObject("object/mount.obj");
		s_mesh->SetTexture(s_texture);
	}

	m_model = new ModelObject(s_mesh);
	m_model->GetTransform().SetParent(&GetTransform());
}

Mountain::~Mountain()
{
}

void Mountain::SetActive(bool active)
{
	Object::SetActive(active);
	m_model->SetActive(active);
}