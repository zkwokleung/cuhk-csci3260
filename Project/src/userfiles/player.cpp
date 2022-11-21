#include "player.h"

Player::Player(void)
	: Object(),
	m_camera(new PerspectiveCamera()),
	m_light(new PointLight(glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), 1, .007f, .008f)), m_travelSpeed(0.f),
	m_velocity(glm::vec3(.0f)), m_translationState(PLAYER_STATE_IDLE), m_rollingSpeed(0.f), m_horizontalSpeed(0.f),
	m_horizontalState(PLAYER_STATE_IDLE), m_rollingState(PLAYER_STATE_IDLE)
{
	// Set camera
	m_camera->GetTransform().SetLocalPosition(glm::vec3(0.f, 5.0f, 20.f));
	m_camera->GetTransform().SetLocalRotation(glm::vec3(.0f));
	m_camera->GetTransform().SetParent(&GetTransform());

	// Set Lighting
	m_light->GetTransform().SetParent(&GetTransform());
	m_light->SetAmbient(glm::vec3(1.f, 1.f, .0f));
	m_light->SetPointLightParams(1.f, 0.07f, 0.08f);

	// Set model
	Mesh* spaceCraft = Resources::LoadObject("object/spacecraft.obj");
	spaceCraft->SetTexture(new Texture(Resources::LoadImageData("texture/spacecraftTexture.bmp")));
	m_model = new ModelObject(spaceCraft);
	m_model->GetTransform().SetLocalPosition(glm::vec3(.0f));
	m_model->GetTransform().SetLocalScale(glm::vec3(.01f));
	m_model->GetTransform().SetLocalRotation(glm::vec3(.0f, 180.f, .0f));
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

	// Yaw (Rotate about up direction)
	glm::vec3 deltaRot = glm::vec3();
	deltaRot += deltaPos.x * PLAYER_ROTATION_SPEED * -1.f * glm::vec3(0.f, 1.f, 0.f);
	// Pitch (Rotate about right direction)
	deltaRot += deltaPos.y * PLAYER_ROTATION_SPEED * -1.f * glm::vec3(1.f, 0.f, 0.f);

	deltaRot = s_activePlayer->GetTransform().GetTransformMat4() * glm::vec4(deltaRot, 1.f);

	// Set the rotation of the player
	s_activePlayer->GetTransform().Rotate(deltaRot);
}

void Player::key_callback(unsigned char key, unsigned int action, int x, int y)
{
	if (s_activePlayer == nullptr)
	{
		return;
	}

	// Control Translation
	if (action == KEYBOARD_ACTION_PRESS || action == KEYBOARD_ACTION_DOWN)
	{
		switch (key)
		{
		case 'w':
			s_activePlayer->m_translationState = PLAYER_STATE_FORWARD;
			break;

		case 'a':
			s_activePlayer->m_horizontalState = PLAYER_STATE_MOVELEFT;
			break;

		case 's':
			s_activePlayer->m_translationState = PLAYER_STATE_BACKWARD;
			break;

		case 'd':
			s_activePlayer->m_horizontalState = PLAYER_STATE_MOVERIGHT;
			break;

		case 'q':
			s_activePlayer->m_rollingState = PLAYER_STATE_ROLLLEFT;
			break;

		case 'e':
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
			if (s_activePlayer->m_horizontalState == PLAYER_STATE_MOVELEFT)
			{
				s_activePlayer->m_horizontalState = PLAYER_STATE_IDLE;
			}
			break;

		case 's':
			if (s_activePlayer->m_translationState == PLAYER_STATE_BACKWARD)
			{
				s_activePlayer->m_translationState = PLAYER_STATE_IDLE;
			}
			break;

		case 'd':
			if (s_activePlayer->m_horizontalState == PLAYER_STATE_MOVERIGHT)
			{
				s_activePlayer->m_horizontalState = PLAYER_STATE_IDLE;
			}
			break;

		case 'q':
			if (s_activePlayer->m_rollingState == PLAYER_STATE_ROLLLEFT)
			{
				s_activePlayer->m_rollingState = PLAYER_STATE_IDLE;
			}
			break;

		case 'e':
			if (s_activePlayer->m_rollingState == PLAYER_STATE_ROLLRIGHT)
			{
				s_activePlayer->m_rollingState = PLAYER_STATE_IDLE;
			}
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

void Player::OnUpdate(void)
{
	static glm::vec3 zDir = glm::vec3();
	static glm::vec3 xDir = glm::vec3();
	static float tT = 0.f;
	static float rT = 0.f;

	// Handle the translation state
	switch (m_translationState)
	{
	case PLAYER_STATE_IDLE:
		// Decelerate the spacecraft if it is moving
		if (m_travelSpeed >= PLAYER_TRANSLATION_DECELERATION)
		{
			m_travelSpeed -= PLAYER_TRANSLATION_DECELERATION;
		}
		else if (m_travelSpeed <= -PLAYER_TRANSLATION_DECELERATION)
		{
			m_travelSpeed += PLAYER_TRANSLATION_DECELERATION;
		}
		else
		{
			m_travelSpeed = 0;
		}
		break;

	case PLAYER_STATE_FORWARD:
		// Accelerate the space craft
		if (m_travelSpeed < PLAYER_MAX_TRAVEL_SPEED)
		{
			m_travelSpeed += PLAYER_TRANSLATION_ACCELERATION;
		}
		zDir = GetTransform().GetForward();
		break;

	case PLAYER_STATE_BACKWARD:
		if (m_travelSpeed > -PLAYER_MAX_TRAVEL_SPEED)
		{
			m_travelSpeed -= PLAYER_TRANSLATION_ACCELERATION;

		}
		zDir = GetTransform().GetForward();
		break;
	}

	// Handle the horizontal state
	switch (m_horizontalState)
	{
	case PLAYER_STATE_IDLE:
		if (m_horizontalSpeed >= PLAYER_TRANSLATION_DECELERATION)
		{
			m_horizontalSpeed -= PLAYER_TRANSLATION_DECELERATION;
		}
		else if (m_horizontalSpeed <= -PLAYER_TRANSLATION_DECELERATION)
		{
			m_horizontalSpeed += PLAYER_TRANSLATION_DECELERATION;
		}
		else
		{
			m_horizontalSpeed = 0;
		}
		break;

	case PLAYER_STATE_MOVELEFT:
		// Accelerate the space craft
		if (m_horizontalSpeed > -PLAYER_MAX_TRAVEL_SPEED)
		{
			m_horizontalSpeed -= PLAYER_TRANSLATION_ACCELERATION;
		}
		xDir = GetTransform().GetRight();
		break;

	case PLAYER_STATE_MOVERIGHT:
		if (m_horizontalSpeed < PLAYER_MAX_TRAVEL_SPEED)
		{
			m_horizontalSpeed += PLAYER_TRANSLATION_ACCELERATION;
		}
		xDir = GetTransform().GetRight();
		break;
	}
	m_velocity = (Time::GetDeltaTime() * m_travelSpeed * zDir) + (Time::GetDeltaTime() * m_horizontalSpeed * xDir);

	// Move the spacecraft
	GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() + m_velocity);

	// Handle Rolling
	static glm::vec3 tranDir = glm::vec3();
	m_velocity = Time::GetDeltaTime() * m_travelSpeed * tranDir;

	// Handle the rotation state
	switch (m_rollingState)
	{
	case PLAYER_STATE_IDLE:
		// Decelerate the spacecraft if it is rolling
		if (m_rollingSpeed > 1)
		{
			m_rollingSpeed -= PLAYER_ROLLING_DECELERATION;
		}
		else if (m_rollingSpeed < -1)
		{
			m_rollingSpeed -= -PLAYER_ROLLING_DECELERATION;
		}
		else
		{
			m_rollingSpeed = 0;
		}
		break;

	case PLAYER_STATE_ROLLLEFT:
		if (m_rollingSpeed > -PLAYER_MAX_ROLLING_SPEED)
		{
			m_rollingSpeed += -PLAYER_ROLLING_ACCELERATION;
		}
		break;

	case PLAYER_STATE_ROLLRIGHT:
		if (m_rollingSpeed < PLAYER_MAX_ROLLING_SPEED)
		{
			m_rollingSpeed += PLAYER_ROLLING_ACCELERATION;
		}
		break;
	}

	// Roll the spacecraft
	glm::vec3 rollingVel = m_rollingSpeed * Time::GetDeltaTime() * GetTransform().GetForward();
	GetTransform().Rotate(rollingVel);

	std::stringstream msg;
	msg << "Up vector: " << s_activePlayer->m_model->GetTransform().GetForward().x << ", "
		<< s_activePlayer->m_model->GetTransform().GetForward().y << ", "
		<< s_activePlayer->m_model->GetTransform().GetForward().z;
	Debug::Log(msg.str());
}

Player* Player::s_activePlayer = nullptr;