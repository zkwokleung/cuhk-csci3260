#include "laser.h"

Model* Laser::s_mesh = nullptr;
Texture* Laser::s_texture = nullptr;

Laser::Laser() : Object("laser"), m_light(new PointLight())
{
	// Initialize the static model and texture if havent
	if (s_texture == nullptr)
	{
		s_texture = new Texture(Resources::LoadImageData("texture/laser.jpg"));
	}
	if (s_mesh == nullptr)
	{
		s_mesh = Resources::LoadObject("object/laser.obj");
		s_mesh->SetTexture(s_texture);
	}

	m_model = new ModelObject(s_mesh);
	m_model->GetTransform().SetParent(&GetTransform());
	m_light->GetTransform().SetParent(&GetTransform());
}

Laser::~Laser()
{
	delete m_model;
	delete m_light;
}

void Laser::SetActive(bool active)
{
	Object::SetActive(active);
	m_model->SetActive(active);
	m_light->SetActive(active);
}

void Laser::OnEnable(void)
{
	Object::OnEnable();

	GetTransform().SetLocalScale(glm::vec3(2.f, 2.f, 10.f));

	// Initialize Point Light
	m_light->GetTransform().SetParent(&GetTransform());
	m_light->GetTransform().SetLocalPosition(glm::vec3(.0f, 2.f, .0f));
	m_light->SetAmbient(glm::vec3(
		Random::Range(0.f, 1.f),
		Random::Range(0.f, 1.f),
		Random::Range(0.f, 1.f)
	));
	m_light->SetPointLightParams(1.f, 0.000045f, 0.000001f);;
}

void Laser::OnUpdate(void)
{
	Object::OnUpdate();

	// Move forward
	GetTransform().Translate(LASER_SPEED * Time::GetDeltaTime() * GetTransform().GetForward());

	// Count down
	static float countDown = LASER_LIFE;

	if (countDown <= 0)
	{
		// Destroy the bullet
		//this->SetActive(false);
		//delete this;
	}
	else
	{
		// keep on counting
		countDown -= Time::GetDeltaTime();
	}
}
