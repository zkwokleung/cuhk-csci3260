#include "craft.h"

Mesh* Craft::s_mesh = nullptr;
Texture* Craft::s_texture = nullptr;

Craft::Craft() : m_light(new PointLight())
{
	// Initialize the static model and texture if havent
	if (s_texture == nullptr)
	{
		s_texture = new Texture(Resources::LoadImageData("texture/vehicleTexture.bmp"));
	}
	if (s_mesh == nullptr)
	{
		s_mesh = Resources::LoadObject("object/craft.obj");
		s_mesh->SetTexture(s_texture);
	}

	m_model = new ModelObject(s_mesh);
	m_model->GetTransform().SetParent(&GetTransform());
	m_light->GetTransform().SetParent(&GetTransform());
}

Craft::~Craft()
{
}

void Craft::SetActive(bool active)
{
	Object::SetActive(active);
	m_model->SetActive(active);
	m_light->SetActive(active);
}

void Craft::OnEnable(void)
{
	Object::OnEnable();

	// Random scale
	glm::vec3 scl = glm::vec3(Random::Range(SPACE_CRAFT_MIN_SCALE, SPACE_CRAFT_MAX_SCALE));
	GetTransform().SetLocalScale(scl);

	GetTransform().SetLocalPosition(
		glm::vec3(
			Random::Range(SPACE_CRAFT_MIN_POSITION_X, SPACE_CRAFT_MAX_POSITION_X),
			Random::Range(SPACE_CRAFT_MIN_POSITION_Y, SPACE_CRAFT_MAX_POSITION_Y),
			Random::Range(SPACE_CRAFT_MIN_POSITION_Z, SPACE_CRAFT_MAX_POSITION_Z)
		)
	);

	// Initialize Point Light
	m_light->GetTransform().SetParent(&GetTransform());
	m_light->GetTransform().SetLocalPosition(glm::vec3(.0f, 2.f, .0f));
	m_light->SetAmbient(glm::vec3(
		Random::Range(0.f, 1.f),
		Random::Range(0.f, 1.f),
		Random::Range(0.f, 1.f)
	));
	m_light->SetPointLightParams(1.f, 0.0045f, 0.0001f);;
}

void Craft::OnUpdate(void)
{
	Object::OnUpdate();

	// Count down and teleport
	static float countDown = 0;
	// Teleport the craft if times up
	if (countDown <= 0)
	{
		// Random location
		glm::vec3 newPos = glm::vec3(
			Random::Range(SPACE_CRAFT_MIN_POSITION_X, SPACE_CRAFT_MAX_POSITION_X),
			Random::Range(SPACE_CRAFT_MIN_POSITION_Y, SPACE_CRAFT_MAX_POSITION_Y),
			Random::Range(SPACE_CRAFT_MIN_POSITION_Z, SPACE_CRAFT_MAX_POSITION_Z)
		);
		GetTransform().SetLocalPosition(newPos);

		countDown = SPACE_CRAFT_TELEPORT_INTERVAL;
	}
	else
	{
		// keep on counting
		countDown -= Time::GetDeltaTime();
	}

	// self-rotate
	GetTransform().Rotate(SPACE_CRAFT_SELF_ROTATE_SPEED * Time::GetDeltaTime() * glm::vec3(0.f, 1.f, 0.f));

}
