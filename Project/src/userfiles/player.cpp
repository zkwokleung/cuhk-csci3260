#include "player.h"

Player::Player(void) : m_camera(new PerspectiveCamera()),
m_light(new PointLight(glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), 1, .7f, 1.8f)),
m_translationAcceleration(.001f), m_rotationAcceleration(.001f)
{
	// Set camera
	m_camera->GetTransform().SetParent(&GetTransform());

	// Set Lighting
	m_light->GetTransform().SetParent(&GetTransform());

	// Set model
	Mesh* spaceCraft = Resources::LoadObject("object/spacecraft.obj");
	spaceCraft->SetTexture(new Texture(Resources::LoadImageData("texture/spacecraftTexture.bmp")));
	m_model = new ModelObject(spaceCraft);
	m_model->GetTransform().SetLocalPosition(glm::vec3(.0f, -10.f, -50.f));
	m_model->GetTransform().SetLocalScale(glm::vec3(.05f));
	m_model->GetTransform().SetParent(&GetTransform());
}

Player::~Player()
{
}

void Player::OnPaint(Shader* shader)
{
}

void Player::cursor_position_callback(int x, int y)
{
	if (s_activePlayer == nullptr)
	{
		return;
	}

	// handle the mouse input
	glm::vec2 newPos = glm::vec2(x, y);
	glm::vec2 deltaPos = newPos - glm::vec2((glutGet(GLUT_WINDOW_WIDTH) / 2), glutGet(GLUT_WINDOW_HEIGHT) / 2);

	glm::vec3 rotate = s_activePlayer->GetTransform().GetRotation();
	rotate += glm::vec3(deltaPos.y * s_activePlayer->m_rotationAcceleration * -1.f, .0f, .0f);
	rotate += glm::vec3(.0f, deltaPos.x * s_activePlayer->m_rotationAcceleration * -1.f, .0f);

	// Clamp the rotation value
	if (rotate.x >= 360.f || rotate.x <= -360.f)
		rotate.x = .0f;
	if (rotate.y >= 360.f || rotate.y <= -360.f)
		rotate.y = .0f;

	// Set the rotation of the camera
	s_activePlayer->GetTransform().SetRotation(rotate);
}

bool Player::IsActive() const
{
	return Player::s_activePlayer == this;
}

void Player::SetActive(bool active)
{
	Object::SetActive(active);
	m_light->SetActive(active);
	m_model->SetActive(active);

	Input::SetCursorLock(active);
	Input::SetCursorDisplay(!active);

	if (active)
	{
		Input::AddCursorPosCallback(cursor_position_callback);

		if (s_activePlayer && s_activePlayer != this)
		{
			// Disable the current active player
			s_activePlayer->SetActive(false);
		}

		// Activate this player
		s_activePlayer = this;
		Camera::SetMain(m_camera);
	}
	else if (s_activePlayer == this)
	{
		Input::RemoveCursorPosCallback(cursor_position_callback);

		// Remove the current active player
		s_activePlayer = nullptr;
		Camera::SetMain(nullptr);
	}
}

Player* Player::s_activePlayer = nullptr;