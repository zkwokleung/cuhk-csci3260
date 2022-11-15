#include "player.h"

Player::Player(void) : m_camera(new PerspectiveCamera()),
m_light(new PointLight(glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), 1, .007f, .008f)),
m_travelSpeed(0.f), m_velocity(glm::vec3(.0f)), m_translationState(PLAYER_STATE_IDLE),
m_rollingSpeed(0.f), m_rollingState(PLAYER_STATE_IDLE)
{
	// Set camera
	m_camera->GetTransform().SetLocalPosition(glm::vec3(0.f));
	m_camera->GetTransform().SetLocalRotation(glm::vec3(.0f));
	m_camera->GetTransform().SetParent(&GetTransform());

	// Set Lighting
	m_light->GetTransform().SetParent(&GetTransform());

	// Set model
	Mesh* spaceCraft = Resources::LoadObject("object/spacecraft.obj");
	spaceCraft->SetTexture(new Texture(Resources::LoadImageData("texture/spacecraftTexture.bmp")));
	m_model = new ModelObject(spaceCraft);
	m_model->GetTransform().SetLocalPosition(glm::vec3(.0f, -5.f, -20.f));
	m_model->GetTransform().SetLocalScale(glm::vec3(.01f));
	m_model->GetTransform().SetParent(&GetTransform());
}

Player::~Player()
{
}

void Player::OnPaint(Shader* shader)
{
	// Behaviour base on state
	switch (m_translationState)
	{
	case PLAYER_STATE_IDLE:
		// Decelerate the spacecraft if it is moving
		if (m_travelSpeed > 0)
		{
			m_travelSpeed -= PLAYER_TRANSLATION_DECELERATION;
		}
		else if (m_travelSpeed < 0)
		{
			m_travelSpeed += PLAYER_TRANSLATION_DECELERATION;
		}
		break;

	case PLAYER_STATE_FORWARD:
		m_velocity = GetTransform().GetForward();
		break;

	case PLAYER_STATE_BACKWARD:
		m_velocity = GetTransform().GetBackward();
		break;

	default:

		break;
	}

	switch (m_rollingState)
	{
	case PLAYER_STATE_IDLE:
		// Decelerate the spacecraft if it is rolling
		if (m_travelSpeed > 0)
		{
			m_travelSpeed -= PLAYER_TRANSLATION_DECELERATION;
		}
		else if (m_travelSpeed < 0)
		{
			m_travelSpeed += PLAYER_TRANSLATION_DECELERATION;
		}
		break;

	case PLAYER_STATE_ROLLLEFT:

		break;

	case PLAYER_STATE_ROLLRIGHT:

		break;

	default:

		break;
	}
	m_velocity *= m_travelSpeed;

	// Accelerate the space craft
	if (m_travelSpeed < PLAYER_MAX_TRAVEL_SPEED)
	{
		m_travelSpeed += PLAYER_TRANSLATION_ACCELERATION;
	}

	// Move the spacecraft
	GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() += m_velocity);
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
	rotate += glm::vec3(deltaPos.y * PLAYER_ROTATION_SPEED * -1.f, .0f, .0f);
	rotate += glm::vec3(.0f, deltaPos.x * PLAYER_ROTATION_SPEED * -1.f, .0f);

	// Clamp the rotation value
	if (rotate.x >= 360.f || rotate.x <= -360.f)
		rotate.x = .0f;
	if (rotate.y >= 360.f || rotate.y <= -360.f)
		rotate.y = .0f;

	// Set the rotation of the camera
	s_activePlayer->GetTransform().SetLocalRotation(rotate);
}

void Player::key_callback(unsigned char key, unsigned int action, int x, int y)
{
	if (s_activePlayer == nullptr)
	{
		return;
	}

	if (action == KEYBOARD_ACTION_PRESS || action == KEYBOARD_ACTION_DOWN)
	{
		switch (key)
		{
		case 'w':
			s_activePlayer->m_translationState = PLAYER_STATE_FORWARD;
			break;

		case 'a':
			s_activePlayer->m_rollingState = PLAYER_STATE_ROLLLEFT;
			break;

		case 's':
			s_activePlayer->m_translationState = PLAYER_STATE_BACKWARD;
			break;

		case 'd':
			s_activePlayer->m_rollingState = PLAYER_STATE_ROLLRIGHT;
			break;
		}
	}
	else if (action == KEYBOARD_ACTION_RELEASE)
	{
		switch (key)
		{
		case 'w':
			if (s_activePlayer->m_translationState == PLAYER_STATE_FORWARD)
			{
				s_activePlayer->m_translationState = PLAYER_STATE_IDLE;
			}
			break;

		case 'a':
			if (s_activePlayer->m_rollingState == PLAYER_STATE_ROLLLEFT)
			{
				s_activePlayer->m_rollingState = PLAYER_STATE_IDLE;
			}
			break;

		case 's':
			if (s_activePlayer->m_translationState == PLAYER_STATE_BACKWARD)
			{
				s_activePlayer->m_translationState = PLAYER_STATE_IDLE;
			}
			break;

		case 'd':
			if (s_activePlayer->m_rollingState == PLAYER_STATE_ROLLRIGHT)
			{
				s_activePlayer->m_rollingState = PLAYER_STATE_IDLE;
			}
			break;
		}
	}
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
		Input::AddKeyCallback(key_callback);

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
		Input::RemoveKeyCallback(key_callback);

		// Remove the current active player
		s_activePlayer = nullptr;
		Camera::SetMain(nullptr);
	}
}

Player* Player::s_activePlayer = nullptr;