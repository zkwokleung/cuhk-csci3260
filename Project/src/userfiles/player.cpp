#include "player.h"


bool swapTexture = false;
bool closeAirCraftLight = false;
Player::Player(void)
	: Object(),
	m_camera(new PerspectiveCamera()),
	m_light(new PointLight(glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), 1, .007f, .008f)), m_travelSpeed(0.f),
	m_velocity(glm::vec3(.0f)), m_translationState(PLAYER_STATE_IDLE), m_rollingSpeed(0.f), m_horizontalSpeed(0.f),
	m_horizontalState(PLAYER_STATE_IDLE), m_rollingState(PLAYER_STATE_IDLE), m_verticalState(PLAYER_STATE_IDLE)
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
	Model* spaceCraft = Resources::LoadObject("object/spacecraft.obj");
	spaceCraft->SetTexture(new Texture(Resources::LoadImageData("texture/spacecraftTexture.bmp")));
	m_model = new ModelObject(spaceCraft);
	m_model->GetTransform().SetLocalPosition(glm::vec3(.0f));
	m_model->GetTransform().SetLocalScale(glm::vec3(.01f));
	m_model->GetTransform().SetLocalRotation(glm::vec3(.0f, 180.f, .0f));
	m_model->GetTransform().SetParent(&GetTransform());


	// Set Collider
	SphereCollider* collider = new SphereCollider(1.f);
	AddComponent(collider);
	collider->AddCollisionCallback(this);
	collider->SetEnabled(true);
}

Player::~Player()
{
}

void Player::OnEnable(void)
{
	Object::OnEnable();

	m_light->SetActive(true);
	m_model->SetActive(true);

	Input::SetCursorLock(true);
	Input::SetCursorDisplay(false);

	Input::AddCursorPosCallback(this);
	Input::AddKeyCallback(this);
	Input::AddMouseButtonCallback(this);

	// Activate this player
	Camera::SetMain(m_camera);
}

void Player::OnDisable(void)
{
	Object::OnDisable();

	m_light->SetActive(false);
	m_model->SetActive(false);

	Input::SetCursorLock(false);
	Input::SetCursorDisplay(true);

	Input::RemoveCursorPosCallback(this);
	Input::RemoveKeyCallback(this);
	Input::RemoveMouseButtonCallback(this);

	Camera::SetMain(nullptr);
}

void Player::OnSwapTexture(void)
{
	if (swapTexture == false)
	{
		m_model->GetMesh()->SetTexture(new Texture(Resources::LoadImageData("texture/spacecraftTexture.bmp")));
	}
	else
	{
		m_model->GetMesh()->SetTexture(new Texture(Resources::LoadImageData("texture/spacecraftTexture2.bmp")));
	}
}

void Player::OnUpdate(void)
{
	static glm::vec3 xDir = glm::vec3();
	static glm::vec3 zDir = glm::vec3();
	static glm::vec3 yDir = glm::vec3();
	static float tT = 0.f;
	static float rT = 0.f;

	/*std::stringstream msg;
	msg << "Forward vector: " << GetTransform().GetLocalPosition().x << ", "
		<< GetTransform().GetLocalPosition().y << ", "
		<< GetTransform().GetLocalPosition().z << std::endl;
	Debug::Log(msg.str());*/


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

	// Handle the vertical state
	switch (m_verticalState)
	{
	case PLAYER_STATE_IDLE:
		if (m_verticalSpeed >= PLAYER_TRANSLATION_DECELERATION)
		{
			m_verticalSpeed -= PLAYER_TRANSLATION_DECELERATION;
		}
		else if (m_verticalSpeed <= -PLAYER_TRANSLATION_DECELERATION)
		{
			m_verticalSpeed += PLAYER_TRANSLATION_DECELERATION;
		}
		else
		{
			m_verticalSpeed = 0;
		}
		break;

	case PLAYER_STATE_UPWARD:
		// Accelerate the space craft
		if (m_verticalSpeed < PLAYER_MAX_TRAVEL_SPEED)
		{
			m_verticalSpeed += PLAYER_TRANSLATION_ACCELERATION;
		}
		yDir = GetTransform().GetUp();
		break;

	case PLAYER_STATE_DOWNWARD:
		if (m_verticalSpeed > -PLAYER_MAX_TRAVEL_SPEED)
		{
			m_verticalSpeed -= PLAYER_TRANSLATION_ACCELERATION;
		}
		yDir = GetTransform().GetUp();
		break;
	}
	m_velocity = (Time::GetDeltaTime() * m_travelSpeed * zDir) +
		(Time::GetDeltaTime() * m_horizontalSpeed * xDir) +
		(Time::GetDeltaTime() * m_verticalSpeed * yDir);

	// Move the spacecraft
	GetTransform().Translate(m_velocity);

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

	//std::stringstream msg;
	//msg << "Forward vector: " << GetTransform().GetForward().x << ", "
	//	<< GetTransform().GetForward().y << ", "
	//	<< GetTransform().GetForward().z << std::endl;
	//Debug::Log(msg.str());
	//msg << "Up vector: " << GetTransform().GetUp().x << ", "
	//	<< GetTransform().GetUp().y << ", "
	//	<< GetTransform().GetUp().z << std::endl;
	//Debug::Log(msg.str());
	//msg << "Right vector: " << GetTransform().GetRight().x << ", "
	//	<< GetTransform().GetRight().y << ", "
	//	<< GetTransform().GetRight().z << std::endl;
	//Debug::Log(msg.str());
}

void Player::OnPaint(Shader* shader)
{
}

void Player::OnCollision(Collider* self, Collider* other)
{
	if (other->GetObject()->GetName() == "planet")
	{
		// Switch to ocean scene
		SceneManager::SetActiveScene(1);
	}
}

void Player::cursor_position_callback(int x, int y)
{
	// handle the mouse input
	glm::vec2 newPos = glm::vec2(x, y);
	glm::vec2 deltaPos = newPos - glm::vec2((glutGet(GLUT_WINDOW_WIDTH) / 2), glutGet(GLUT_WINDOW_HEIGHT) / 2);

	// Yaw (Rotate about up direction)
	glm::vec3 deltaRot = glm::vec3();
	deltaRot += Time::GetDeltaTime() * deltaPos.x * PLAYER_SENSITIVITY * -1.f * GetTransform().GetUp();
	// Pitch (Rotate about right direction)
	//deltaRot += Time::GetDeltaTime() * deltaPos.y * PLAYER_SENSITIVITY * -1.f * GetTransform().GetRight();

	// Set the rotation of the player
	GetTransform().Rotate(deltaRot);
}

void Player::key_callback(unsigned char key, unsigned int action, int x, int y)
{
	// Control Translation
	if (action == KEYBOARD_ACTION_PRESS || action == KEYBOARD_ACTION_DOWN)
	{
		switch (key)
		{
		case 'W':
		case 'w':
			m_translationState = PLAYER_STATE_FORWARD;
			break;

		case 'A':
		case 'a':
			m_horizontalState = PLAYER_STATE_MOVELEFT;
			break;
		case 'S':
		case 's':
			m_translationState = PLAYER_STATE_BACKWARD;
			break;

		case 'D':
		case 'd':
			m_horizontalState = PLAYER_STATE_MOVERIGHT;
			break;

			//case 'q':
			//	m_rollingState = PLAYER_STATE_ROLLLEFT;
			//	break;

			//case 'e':
			//	m_rollingState = PLAYER_STATE_ROLLRIGHT;
			//	break;

		case 'Z':
		case 'z':
			m_verticalState = PLAYER_STATE_UPWARD;
			break;

		case 'X':
		case 'x':
			m_verticalState = PLAYER_STATE_DOWNWARD;
			break;

		case 'C':
		case 'c':
			if ((GetTransform().GetLocalPosition().x < 2644) && (GetTransform().GetLocalPosition().x > -3751) && (GetTransform().GetLocalPosition().y < -300) && (GetTransform().GetLocalPosition().y > -1000) && (GetTransform().GetLocalPosition().z > 13860) && (GetTransform().GetLocalPosition().z < 15968)) {
				if (swapTexture == false)
				{
					swapTexture = true;
					OnSwapTexture();
				}
				else
				{
					swapTexture = false;
					OnSwapTexture();
				}
			}
			break;
		case 'B':
		case 'b':
			if (closeAirCraftLight == false)
			{
				closeAirCraftLight = true;
				m_light->SetAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
			}
			else
			{
				closeAirCraftLight = false;
				m_light->SetAmbient(glm::vec3(1.0f, 1.0f, 0.0f));
			}

			break;

		}
	}
	else if (action == KEYBOARD_ACTION_RELEASE)
	{
		switch (key)
		{
		case 'W':
		case 'w':
			if (m_translationState == PLAYER_STATE_FORWARD)
			{
				m_translationState = PLAYER_STATE_IDLE;
			}
			break;

		case 'A':
		case 'a':
			if (m_horizontalState == PLAYER_STATE_MOVELEFT)
			{
				m_horizontalState = PLAYER_STATE_IDLE;
			}
			break;

		case 'S':
		case 's':
			if (m_translationState == PLAYER_STATE_BACKWARD)
			{
				m_translationState = PLAYER_STATE_IDLE;
			}
			break;

		case 'D':
		case 'd':
			if (m_horizontalState == PLAYER_STATE_MOVERIGHT)
			{
				m_horizontalState = PLAYER_STATE_IDLE;
			}
			break;

			//case 'q':
			//	if (m_rollingState == PLAYER_STATE_ROLLLEFT)
			//	{
			//		m_rollingState = PLAYER_STATE_IDLE;
			//	}
			//	break;

			//case 'e':
			//	if (m_rollingState == PLAYER_STATE_ROLLRIGHT)
			//	{
			//		m_rollingState = PLAYER_STATE_IDLE;
			//	}
			//	break;

		case 'Z':
		case 'z':
			if (m_verticalState == PLAYER_STATE_UPWARD)
			{
				m_verticalState = PLAYER_STATE_IDLE;
			}
			break;

		case 'X':
		case 'x':
			if (m_verticalState == PLAYER_STATE_DOWNWARD)
			{
				m_verticalState = PLAYER_STATE_IDLE;
			}
			break;

		}
	}
}

void Player::mouse_button_callbak(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// Shoot laser
		Laser* l = new Laser();
		l->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition());
		l->GetTransform().SetLocalRotation(GetTransform().GetLocalRotation());
		l->SetActive(true);
	}
}
