#include "firstPersonPlayer.h"



PlayerController::PlayerController() : m_velocity(glm::vec3()), m_camera(nullptr), m_body(nullptr), m_speed(.1f), m_controlSpeed(.1f)
{
	static bool init = false;
	if (!init)
	{
		Input::AddKeyCallback(PlayerController::key_callback);
		Input::AddCursorPosCallback(PlayerController::cursor_pos_callback);
	}
}

PlayerController::~PlayerController()
{
}

void PlayerController::SetVelocity(glm::vec3 value)
{
	m_velocity = value;
}

glm::vec3 PlayerController::GetVelocity() const
{
	return glm::vec3();
}

void PlayerController::SetActive(bool active)
{
	Object::SetActive(active);

	if (active)
	{
		s_activeController = this;
		Camera::SetMain(m_camera);
	}
	else if (s_activeController == this)
	{
		s_activeController = nullptr;
		Camera::SetMain(nullptr);
	}

	Input::SetCursorLock(active);
}

bool PlayerController::IsActive() const
{
	return PlayerController::s_activeController == this;
}

void PlayerController::OnPaint(Shader* shader)
{
	m_body->SetPosition(m_body->GetPosition() + m_velocity);
}

void PlayerController::SetCamera(Camera* camera)
{
	m_camera = camera;
}

void PlayerController::SetBody(Transform* body)
{
	m_body = body;
}

PlayerController* PlayerController::s_activeController = nullptr;

void PlayerController::key_callback(unsigned char key, unsigned int action, int x, int y)
{
	if (s_activeController == nullptr)
	{
		return;
	}

	if (action == KEYBOARD_ACTION_DOWN || action == KEYBOARD_ACTION_PRESS)
	{
		switch (key)
		{
		case 'w':
			s_activeController->m_velocity = s_activeController->m_body->GetForward();
			break;

		case 'a':
			s_activeController->m_velocity = s_activeController->m_body->GetLeft();
			break;

		case 'd':
			s_activeController->m_velocity = s_activeController->m_body->GetRight();
			break;

		case 's':
			s_activeController->m_velocity = s_activeController->m_body->GetBackward();
			break;
		}
	}
	else if (action == KEYBOARD_ACTION_RELEASE)
	{
		s_activeController->m_velocity = glm::vec3();
	}
}

void PlayerController::cursor_pos_callback(int x, int y)
{
	if (s_activeController == nullptr)
	{
		return;
	}

	if (x == (glutGet(GLUT_WINDOW_WIDTH) / 2) && y == glutGet(GLUT_WINDOW_HEIGHT) / 2)
	{
		return;
	}

	PlayerController* pc = PlayerController::s_activeController;

	// Get Main Camera
	Camera* cam = pc->m_camera;
	// Get Controller Body
	Transform* body = pc->m_body;

	// handle the mouse input
	glm::vec2 newPos = glm::vec2(x, y);
	glm::vec2 deltaPos = newPos - glm::vec2((glutGet(GLUT_WINDOW_WIDTH) / 2), glutGet(GLUT_WINDOW_HEIGHT) / 2);

	glm::vec3 cameraRotation = cam->GetTransform().GetRotation();
	cameraRotation += glm::vec3(deltaPos.y * pc->m_controlSpeed, .0f, .0f);

	glm::vec3 bodyRotation = body->GetRotation();
	bodyRotation += glm::vec3(.0f, deltaPos.x * pc->m_controlSpeed, .0f);

	// Clamp the rotation value
	if (cameraRotation.x >= 360.f || cameraRotation.x <= -360.f)
		cameraRotation.x = .0f;
	if (bodyRotation.y >= 360.f || bodyRotation.y <= -360.f)
		bodyRotation.y = .0f;

	// Set the rotation of the camera
	cam->GetTransform().SetRotation(cameraRotation);
	body->SetRotation(bodyRotation);
}

FirstPersonPlayer::FirstPersonPlayer() : Object(), m_camera(new Camera()), m_controller(new PlayerController())
{
	m_camera->GetTransform().SetPosition(glm::vec3(.0f, .5f, .0f));
	m_camera->GetTransform().SetParent(&GetTransform());

	m_controller->SetBody(&GetTransform());
	m_controller->SetCamera(m_camera);
}

FirstPersonPlayer::~FirstPersonPlayer()
{
	delete m_camera;
	delete m_controller;
}

void FirstPersonPlayer::SetActive(bool active)
{
	Object::SetActive(active);
	m_controller->SetActive(active);
	if (active)
	{
		Camera::SetMain(m_camera);
	}
	else
	{
		if (Camera::GetMain() == m_camera)
		{
			Camera::SetMain(nullptr);
		}
	}
}

bool FirstPersonPlayer::IsActive() const
{
	return m_isActive;
}
