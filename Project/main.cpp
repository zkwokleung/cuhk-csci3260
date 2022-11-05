/*
Student Information
Student ID: 1155149068
Student Name: SZE-TO Kwok Leung
*/

#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <sstream>

#pragma region Assignment given APIs

// screen setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

#pragma endregion

#pragma region My API
/***************************************************************
	Classes and functions created by me to simplify some codes
****************************************************************/
#pragma region Debug
#pragma endregion

#pragma region Render Related
/**********************
	Render related
***********************/

#pragma endregion

#pragma region Generic
/**********************
	Generic
***********************/

#pragma endregion

#pragma region Camera
/**********************
	Camera related
***********************/
#pragma endregion

#pragma region GameObject Related
/**********************
	Game Object related
***********************/
#pragma endregion

#pragma region Material
#pragma endregion

#pragma region ModelObject
#pragma endregion

#pragma region Scene
#pragma endregion

#pragma region Input Related

#pragma endregion

#pragma region Lighting

// Directional Light
class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction, float intensity);
	~DirectionalLight();

	void SetDirection(glm::vec3 direction);
	glm::vec3 GetDirection() const;

	void SetIntensity(float intensity);
	float GetIntensity() const;

	virtual glm::mat4 GetProjectionMat4();
	virtual glm::mat4 GetViewMat4();

	virtual void OnPaint(Shader* shader);

private:
	glm::vec3 m_direction;
	float m_intensity;

	virtual std::string GetUniformNamePrefix() const;
};

DirectionalLight::DirectionalLight() : Light(), m_direction(), m_intensity(1.f)
{

}
DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction, float intensity) :
	Light(ambient, diffuse, specular), m_direction(direction), m_intensity(intensity)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::SetDirection(glm::vec3 direction)
{
	m_direction = direction;
}

glm::vec3 DirectionalLight::GetDirection() const
{
	return m_direction;
}

void DirectionalLight::SetIntensity(float intensity)
{
	m_intensity = intensity;
}

float DirectionalLight::GetIntensity() const
{
	return m_intensity;
}

void DirectionalLight::OnPaint(Shader* shader)
{
	Light::OnPaint(shader);
	shader->setVec3(GetUniformNamePrefix() + ".direction", m_direction);
	shader->setFloat(GetUniformNamePrefix() + ".intensity", m_intensity);
}

glm::mat4 DirectionalLight::GetProjectionMat4()
{
	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	return lightProjection;
}

glm::mat4 DirectionalLight::GetViewMat4()
{
	return glm::lookAt(GetTransform().GetPosition(), GetDirection(), GetTransform().GetUp());
}

std::string DirectionalLight::GetUniformNamePrefix() const
{
	return "dirLight";
}

// Point Light
class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quaratic);
	~PointLight();

	virtual void OnPaint(Shader* shader);
	virtual std::string GetUniformNamePrefix() const;
	virtual void SetActive(bool active);

	void SetIntensity(float intensity);
	float GetIntensity() const;

private:
	int m_id;
	float m_constant, m_linear, m_quadratic, m_intensity;

	static int s_activeCount;
};

PointLight::PointLight() : Light(), m_constant(.0f), m_linear(.0f), m_quadratic(.0f), m_intensity(1.f), m_id(-1)
{

}
PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quaratic) :
	Light(ambient, diffuse, specular), m_constant(constant), m_linear(linear), m_quadratic(quaratic),
	m_intensity(1.f), m_id(-1)
{
}

PointLight::~PointLight()
{
}

void PointLight::OnPaint(Shader* shader)
{
	Light::OnPaint(shader);
	shader->setFloat(GetUniformNamePrefix() + ".constant", m_constant);
	shader->setFloat(GetUniformNamePrefix() + ".linear", m_linear);
	shader->setFloat(GetUniformNamePrefix() + ".quadratic", m_quadratic);

	shader->setVec3(GetUniformNamePrefix() + ".position", GetTransform().GetPosition());

	shader->setFloat(GetUniformNamePrefix() + ".intensity", m_intensity);

	shader->setInt(GetUniformNamePrefix() + ".isActive", 1);
}

std::string PointLight::GetUniformNamePrefix() const
{
	std::stringstream ss;
	char id = m_id + '0';
	ss << "pointLights[" << id << "]";
	return ss.str();
}

void PointLight::SetActive(bool active)
{
	if (active)
	{
		if (!IsActive())
		{
			m_id = s_activeCount++;
		}
	}
	else
	{
		if (IsActive())
		{
			m_id = -1;
			s_activeCount--;
		}
	}

	Object::SetActive(active);
}

void PointLight::SetIntensity(float intensity)
{
	m_intensity = intensity;
}

float PointLight::GetIntensity() const
{
	return m_intensity;
}

int PointLight::s_activeCount = 0;

// Spot Light
class SpotLight : public PointLight
{
public:
	SpotLight();
	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quaratic,
		glm::vec3 direction, float cutOff, float innerCutOff);
	~SpotLight();

	virtual std::string GetUniformNamePrefix() const;
	virtual void OnPaint(Shader* shader);

	void SetDirection(glm::vec3 direction);
	glm::vec3 GetDirection() const;

	void SetCutOff(float cutOff);
	float GetCutOff() const;
	void SetInnerCutOff(float innerCutOff);
	float GetInnerCutOff() const;

private:
	glm::vec3 m_direction;
	float m_cutOff;
	float m_innerCutOff;
};

SpotLight::SpotLight() : PointLight(), m_direction(glm::vec3(.0f, -1.f, .0f)), m_cutOff(12.5f), m_innerCutOff(0.99f)
{

}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	float constant, float linear, float quaratic,
	glm::vec3 direction, float cutOff, float innerCutOff) :
	PointLight(ambient, diffuse, specular, constant, linear, quaratic),
	m_direction(direction), m_cutOff(cutOff), m_innerCutOff(innerCutOff)
{
}

SpotLight::~SpotLight()
{
}

std::string SpotLight::GetUniformNamePrefix() const
{
	return "spotLight";
}

void SpotLight::OnPaint(Shader* shader)
{
	PointLight::OnPaint(shader);
	shader->setVec3(GetUniformNamePrefix() + ".position", GetTransform().GetPosition());
	shader->setVec3(GetUniformNamePrefix() + ".direction", m_direction);
	shader->setFloat(GetUniformNamePrefix() + ".cutOff", m_cutOff);
	shader->setFloat(GetUniformNamePrefix() + ".innerCutOff", m_innerCutOff);
}

void SpotLight::SetDirection(glm::vec3 direction)
{
	m_direction = direction;
}

glm::vec3 SpotLight::GetDirection() const
{
	return m_direction;
}

void SpotLight::SetCutOff(float cutOff)
{
	m_cutOff = cutOff;
}

float SpotLight::GetCutOff() const
{
	return m_cutOff;
}

void SpotLight::SetInnerCutOff(float innerCutOff)
{
	m_innerCutOff = innerCutOff;
}

float SpotLight::GetInnerCutOff() const
{
	return m_innerCutOff;
}


#pragma endregion

#pragma region Shadow
class ShadowMapper {
public:
	ShadowMapper();
	~ShadowMapper();

	void Bind();
	void Unbind();

private:
	unsigned int m_depthMapFBO;
	unsigned int m_depthMap;

	static const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
};

ShadowMapper::ShadowMapper()
{
	// Freate frame buffer
	glGenFramebuffers(1, &m_depthMapFBO);

	// Create 2D texture
	glGenTextures(1, &m_depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Attach
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMapper::~ShadowMapper()
{
	glDeleteFramebuffers(1, &m_depthMapFBO);
	glDeleteTextures(1, &m_depthMap);
}

void ShadowMapper::Bind()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapper::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#pragma endregion

#pragma endregion

#pragma region Assignment Specific Classes
#pragma region Player Controller
class PlayerController : public Object
{
public:
	PlayerController();
	~PlayerController();

	void SetVelocity(glm::vec3 value);
	glm::vec3 GetVelocity() const;

	virtual void SetActive(bool active);
	virtual bool IsActive() const;

	virtual void OnPaint();

	void SetCamera(Camera* camera);
	void SetBody(Transform* body);

private:
	static PlayerController* s_activeController;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

	float m_speed;
	float m_controlSpeed;
	glm::vec3 m_velocity;
	Camera* m_camera;
	Transform* m_body;
};

PlayerController::PlayerController() : m_velocity(glm::vec3()), m_camera(nullptr), m_body(nullptr), m_speed(.1f), m_controlSpeed(.1f)
{
	static bool init = false;
	if (!init)
	{
		Input::AddKeyCallback(PlayerController::key_callback);
		Input::AddCursorPosCallback(PlayerController::cursor_pos_callback);
		init = true;
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
}

bool PlayerController::IsActive() const
{
	return PlayerController::s_activeController == this;
}

void PlayerController::OnPaint()
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

void PlayerController::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (s_activeController == nullptr)
	{
		return;
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			s_activeController->m_velocity = s_activeController->m_body->GetForward();
			break;

		case GLFW_KEY_A:
			s_activeController->m_velocity = s_activeController->m_body->GetLeft();
			break;

		case GLFW_KEY_D:
			s_activeController->m_velocity = s_activeController->m_body->GetRight();
			break;

		case GLFW_KEY_S:
			s_activeController->m_velocity = s_activeController->m_body->GetBackward();
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		s_activeController->m_velocity = glm::vec3();
	}
}

void PlayerController::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (s_activeController == nullptr)
	{
		return;
	}

	static int wWidth, wHeight; // Window width and height

	// Get the center position of the window
	glfwGetWindowSize(window, &wWidth, &wHeight);
	static glm::vec2 center = glm::vec2(wWidth / 2, wHeight / 2);

	// Do not proceed if the position is the center of the window
	if (xpos == center.x && ypos == center.y)
		return;

	PlayerController* pc = PlayerController::s_activeController;

	// Get Main Camera
	Camera* cam = pc->m_camera;
	// Get Controller Body
	Transform* body = pc->m_body;

	// handle the mouse input
	glm::vec2 newPos = glm::vec2(xpos, ypos);
	glm::vec2 deltaPos = newPos - center;

	glm::vec3 cameraRotation = cam->GetTransform().GetRotation();
	cameraRotation += glm::vec3(deltaPos.y * pc->m_controlSpeed * -1.f, .0f, .0f);

	glm::vec3 bodyRotation = body->GetRotation();
	bodyRotation += glm::vec3(.0f, deltaPos.x * pc->m_controlSpeed * -1.f, .0f);

	// Clamp the rotation value
	if (cameraRotation.x >= 360.f || cameraRotation.x <= -360.f)
		cameraRotation.x = .0f;
	if (bodyRotation.y >= 360.f || bodyRotation.y <= -360.f)
		bodyRotation.y = .0f;

	// Set the rotation of the camera
	cam->GetTransform().SetRotation(cameraRotation);
	body->SetRotation(bodyRotation);

	// Fix the cursor position to the center of the screen
	glfwSetCursorPos(window, wWidth / 2, wHeight / 2);
}

#pragma endregion

#pragma region First Person Player
class FirstPersonPlayer : public Object
{
public:
	FirstPersonPlayer();
	~FirstPersonPlayer();

	virtual void SetActive(bool active);
	virtual bool IsActive() const;

private:
	Camera* m_camera;
	PlayerController* m_controller;
};

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

#pragma endregion

#pragma region Mountain Scene

#define DISCO_LIGHT_COUNT 9
class MountainScene : public Scene
{
public:
	MountainScene();
	~MountainScene();

	virtual void OnInitialize();
	virtual void OnPaint(Shader* shader);
	virtual void OnEnd();

	static MountainScene* GetInstance();

private:
	// Main Objects
	Camera* m_cam;
	ModelObject* m_tiger;
	Object* m_tigerContainer;
	ModelObject* m_ground;

	// Side Objects
	ModelObject* m_cottage;
	ModelObject* m_tower;
	ModelObject* m_car;

	// Mountain
	ModelObject* m_mountain;
	ModelObject* m_mountain2;
	ModelObject* m_mountain3;
	ModelObject* m_mountain4;
	ModelObject* m_mountain5;
	ModelObject* m_mountain6;

	// Textures
	Texture m_tigerTex1;
	Texture m_tigerTex2;
	Texture m_groundTex1;
	Texture m_groundTex2;

	// Lights
	DirectionalLight* m_dirLight;
	PointLight* m_pointLight;
	PointLight* m_discoLights[DISCO_LIGHT_COUNT];
	SpotLight* m_spotLight;

	// Car control variables
	float m_carAngle;
	float m_carSpeed;

	// Disco
	bool m_discoModeOn;

	static MountainScene* s_instance;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, double xpos, int ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static bool s_IsMouseLeftButtonDown;
};
MountainScene::MountainScene() : Scene(), m_cam(new Camera()),
m_tiger(new ModelObject(loadOBJ("resources/tiger/tiger.obj"))), m_tigerContainer(new Object()),
m_ground(new ModelObject(loadOBJ("resources/ground/ground.obj"))),
m_cottage(new ModelObject("resources/cottage/cottage.obj", "resources/cottage/Cottage_Dirt_Base_Color.png")),
m_tower(new ModelObject("resources/tower/tower.obj", "resources/tower/woodenplank.jpg")),
m_car(new ModelObject("resources/car/car.obj", "resources/car/rust.jpg")),
m_mountain(new ModelObject("resources/mountain/mount.obj", "resources/mountain/rock-mountain.png")),
m_mountain2(new ModelObject("resources/mountain/mount.obj", "resources/mountain/rock-mountain.png")),
m_mountain3(new ModelObject("resources/mountain/mount.obj", "resources/mountain/rock-mountain.png")),
m_mountain4(new ModelObject("resources/mountain/mount.obj", "resources/mountain/rock-mountain.png")),
m_mountain5(new ModelObject("resources/mountain/mount.obj", "resources/mountain/rock-mountain.png")),
m_mountain6(new ModelObject("resources/mountain/mount.obj", "resources/mountain/rock-mountain.png")),
m_tigerTex1(), m_tigerTex2(), m_groundTex1(), m_groundTex2(),
m_dirLight(new DirectionalLight(glm::vec3(.5f, .5f, .5f), glm::vec3(.5f, .5f, .5f), glm::vec3(.5f, .5f, .5f), glm::vec3(.5f, .5f, .5f), .5f)),
m_pointLight(new PointLight(glm::vec3(.5f, .5f, .5f), glm::vec3(.5f, .5f, .5f), glm::vec3(.5f, .5f, .5f), 1.0f, 0.7f, 1.8f)),
m_spotLight(new SpotLight()),
m_discoModeOn(false),
m_carAngle(0.f), m_carSpeed(1.f)
{
	// Tiger Texture
	m_tigerTex1.setupTexture("resources/tiger/tiger_01.jpg");
	m_tigerTex2.setupTexture("resources/tiger/tiger_02.jpg");
	m_tiger->SetTexture(&m_tigerTex1);

	// Ground Texture
	m_groundTex1.setupTexture("resources/ground/ground_01.jpg");
	m_groundTex2.setupTexture("resources/ground/ground_02.jpg");
	m_ground->SetTexture(&m_groundTex1);

	// Point Lights
	for (int i = 0; i < DISCO_LIGHT_COUNT; i++)
	{
		m_discoLights[i] = new PointLight(
			glm::vec3(.5f, .5f, .5f), glm::vec3(.5f, .5f, .5f), glm::vec3(.5f, .5f, .5f),
			1.0f, 0.7f, 1.8f);
	}
}

MountainScene::~MountainScene()
{
	delete m_cam;
	delete m_tiger;
	delete m_ground;
	delete m_tigerContainer;
	delete m_cottage;
	delete m_tower;
	delete m_car;
	delete m_mountain;
	delete m_mountain2;
	delete m_mountain3;
}

void MountainScene::OnInitialize()
{
	s_instance = this;
	Input::AddCursorPosCallback(MountainScene::cursor_position_callback);
	Input::AddKeyCallback(MountainScene::key_callback);
	Input::AddMouseButtonCallback(MountainScene::mouse_button_callback);
	Input::AddScrollCallback(MountainScene::scroll_callback);

	// Camera
	Camera::SetMain(m_cam);
	m_cam->GetTransform().SetLocalPosition(glm::vec3(.0f, .5f, .0f));

	// Ground
	m_ground->GetTransform().SetLocalPosition(glm::vec3(0.f, 0.f, 0.f));
	m_ground->GetTransform().SetLocalScale(glm::vec3(10.f, 1.f, 10.f));
	m_ground->SetActive(true);

	// Tiger
	m_tiger->GetTransform().SetParent(&m_tigerContainer->GetTransform());
	m_tiger->GetTransform().SetLocalRotation(glm::vec3(.0f, -135.f, .0f));
	m_tiger->SetActive(true);
	m_tigerContainer->GetTransform().SetLocalPosition(glm::vec3(0, 0, -2.0f));
	m_tigerContainer->SetActive(true);

	// Cottage
	m_cottage->GetTransform().SetLocalPosition(glm::vec3(.0f, -1.f, 10.f));
	m_cottage->GetTransform().SetLocalScale(glm::vec3(.2f, .2f, .2f));
	m_cottage->GetTransform().SetLocalRotation(glm::vec3(.0f, -75.f, .0f));
	m_cottage->SetActive(true);

	// tower
	m_tower->GetTransform().SetLocalPosition(glm::vec3(.0f, -2.f, -8.f));
	m_tower->GetTransform().SetLocalRotation(glm::vec3(.0f, -75.f, .0f));
	m_tower->SetActive(true);

	// Car
	m_car->GetTransform().SetLocalPosition(glm::vec3(.0f, -1.2f, -10.f));
	m_car->GetTransform().SetLocalScale(glm::vec3(.5f, .5f, .5f));
	m_car->GetMaterial().SetAmbient(glm::vec3(1.f, .4f, .4f));
	m_car->SetActive(true);

	// Mountain 
	m_mountain->GetTransform().SetLocalPosition(glm::vec3(.0f, -2.0f, -100.f));
	m_mountain->GetTransform().SetScale(glm::vec3(10.f, 10.f, 10.f));
	m_mountain->SetActive(true);

	m_mountain2->GetTransform().SetLocalPosition(glm::vec3(-100.0f, -5.0f, -100.f));
	m_mountain2->GetTransform().SetScale(glm::vec3(10.f, 10.f, 10.f));
	m_mountain2->SetActive(true);

	m_mountain3->GetTransform().SetLocalPosition(glm::vec3(100.0f, -5.0f, -100.f));
	m_mountain3->GetTransform().SetScale(glm::vec3(10.f, 10.f, 10.f));
	m_mountain3->SetActive(true);

	m_mountain4->GetTransform().SetLocalPosition(glm::vec3(50.0f, -5.0f, 50.f));
	m_mountain4->GetTransform().SetScale(glm::vec3(10.f, 10.f, 10.f));
	m_mountain4->SetActive(true);

	m_mountain5->GetTransform().SetLocalPosition(glm::vec3(-50.0f, -5.0f, -50.f));
	m_mountain5->GetTransform().SetScale(glm::vec3(10.f, 10.f, 10.f));
	m_mountain5->SetActive(true);

	m_mountain6->GetTransform().SetLocalPosition(glm::vec3(-100.0f, -5.0f, 100.f));
	m_mountain6->GetTransform().SetScale(glm::vec3(10.f, 10.f, 10.f));
	m_mountain6->SetActive(true);

	// Directional Light
	m_dirLight->SetActive(true);
	m_dirLight->SetAmbient(glm::vec3(.2f));
	m_dirLight->SetDiffuse(glm::vec3(.2f));
	m_dirLight->SetSpecular(glm::vec3(1.f));
	m_dirLight->SetDirection(glm::vec3(-1.f, -1.f, 1.0f));

	// Point Light
	m_pointLight->SetAmbient(glm::vec3(.0f, 2.0f, 1.0f));
	m_pointLight->GetTransform().SetLocalPosition(glm::vec3(0.f, 5.f, -8.f));
	m_pointLight->SetActive(true);

	// Disco Lights
	m_discoLights[0]->SetAmbient(glm::vec3(.0f, .0f, 1.0f));
	m_discoLights[1]->SetAmbient(glm::vec3(.0f, 1.0f, 1.0f));
	m_discoLights[2]->SetAmbient(glm::vec3(1.0f, 1.0f, .0f));
	m_discoLights[3]->SetAmbient(glm::vec3(1.0f, .0f, .0f));
	m_discoLights[4]->SetAmbient(glm::vec3(1.0f, .0f, 1.0f));
	m_discoLights[5]->SetAmbient(glm::vec3(.5f, 1.0f, .5f));
	m_discoLights[6]->SetAmbient(glm::vec3(.5f, .5f, 1.0f));
	m_discoLights[7]->SetAmbient(glm::vec3(1.0f, .5f, .5f));
	m_discoLights[8]->SetAmbient(glm::vec3(.0f, 1.0f, .0f));

	for (int i = 0; i < DISCO_LIGHT_COUNT; i++)
	{
		glm::vec3 pos = 5.f * glm::vec3(glm::cos(glm::radians(40.f * i)), .0f, glm::sin(glm::radians(40.f * i)));
		pos += glm::vec3(.0f, 1.5f, 10.f);
		m_discoLights[i]->GetTransform().SetLocalPosition(pos);
		m_discoLights[i]->SetIntensity(2.f);
		m_discoLights[i]->SetActive(true);
	}

	// Spot Light
	m_spotLight->SetAmbient(glm::vec3(.0f, .0f, 1.f));
	m_spotLight->GetTransform().SetLocalPosition(glm::vec3(1.f, 5.f, 1.f));
	m_spotLight->SetDirection(glm::vec3(.0f, -1.f, .0f));
	m_spotLight->SetActive(true);
}

void MountainScene::OnPaint(Shader* shader)
{
	// Let the car drift
	m_carAngle += m_carSpeed;
	if (m_carAngle > 360.f)
		m_carAngle = 0.f;
	glm::vec3 newPos = 10.f * glm::vec3(glm::sin(glm::radians(m_carAngle)), 0.f, glm::cos(glm::radians(m_carAngle)));
	newPos += glm::vec3(.0f, -.5f, .0f);
	m_car->GetTransform().SetLocalRotation(glm::vec3(.0f, m_carAngle - 180.f, .0f));
	m_car->GetTransform().SetLocalPosition(newPos);

	// Cycle through disco lights
	if (m_discoModeOn)
	{
		for (int i = 0; i < DISCO_LIGHT_COUNT; i++)
		{
			m_discoLights[i]->SetActive(false);
		}

		static int currentLight = 0;
		currentLight = rand() % DISCO_LIGHT_COUNT;
		m_discoLights[currentLight]->SetActive(true);
	}
}

void MountainScene::OnEnd()
{
}

MountainScene* MountainScene::GetInstance()
{
	return s_instance;
}

void MountainScene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Quit the game
	if (key == GLFW_KEY_ESCAPE)
		glfwDestroyWindow(window);

	if (action == GLFW_PRESS)
	{
		// Press key “w” and key “s” to increase and reduce the brightness of directional light
		if (key == GLFW_KEY_W)
		{
			GetInstance()->m_dirLight->SetIntensity(GetInstance()->m_dirLight->GetIntensity() + .1f);
		}
		else if (key == GLFW_KEY_S)
		{
			GetInstance()->m_dirLight->SetIntensity(GetInstance()->m_dirLight->GetIntensity() - .1f);
		}

		// Press key ’1’ and ’2’ to switch two different textures for the tiger
		if (key == GLFW_KEY_1)
		{
			GetInstance()->m_tiger->SetTexture(&GetInstance()->m_tigerTex1);
		}
		else if (key == GLFW_KEY_2)
		{
			GetInstance()->m_tiger->SetTexture(&GetInstance()->m_tigerTex2);
		}

		// Press key ’3’ and ’4’ to switch two different textures for the ground surface
		else if (key == GLFW_KEY_3)
		{
			GetInstance()->m_ground->SetTexture(&GetInstance()->m_groundTex1);
		}
		else if (key == GLFW_KEY_4)
		{
			GetInstance()->m_ground->SetTexture(&GetInstance()->m_groundTex2);
		}

		static float speed = .5f;
		// Press arrow keys and L to control the movements of the tiger.
		if (key == GLFW_KEY_UP)
		{
			// Move forward
			GetInstance()->m_tigerContainer->GetTransform().SetLocalPosition(
				GetInstance()->m_tigerContainer->GetTransform().GetLocalPosition() + speed * GetInstance()->m_tigerContainer->GetTransform().GetUp());
		}
		else if (key == GLFW_KEY_DOWN)
		{
			// Move backward
			GetInstance()->m_tigerContainer->GetTransform().SetLocalPosition(
				GetInstance()->m_tigerContainer->GetTransform().GetLocalPosition() + speed * GetInstance()->m_tigerContainer->GetTransform().GetDown());
		}
		// left and right arrow indicate left and right rotation respectively
		else if (key == GLFW_KEY_LEFT)
		{
			GetInstance()->m_tigerContainer->GetTransform().SetLocalRotation(GetInstance()->m_tigerContainer->GetTransform().GetLocalRotation() + glm::vec3(.0f, 30.f, .0f));
		}
		else if (key == GLFW_KEY_RIGHT)
		{
			GetInstance()->m_tigerContainer->GetTransform().SetLocalRotation(GetInstance()->m_tigerContainer->GetTransform().GetLocalRotation() + glm::vec3(.0f, -30.f, .0f));
		}
		else if (key == GLFW_KEY_L)
		{
			// ‘L’indicate the random movement in the horizon plane
			int r = rand() % 4;
			glm::vec3 dir = glm::vec3();
			switch (r)
			{
			case 0:
				dir = glm::vec3(speed, .0f, .0f);
				break;

			case 1:
				dir = glm::vec3(-speed, .0f, .0f);
				break;

			case 2:
				dir = glm::vec3(.0f, .0f, speed);
				break;
			default:
				dir = glm::vec3(.0f, .0f, -speed);
			}

			GetInstance()->m_tigerContainer->GetTransform().SetLocalPosition(GetInstance()->m_tigerContainer->GetTransform().GetPosition() + dir);
		}

		// Control Point Light Intensity
		if (key == GLFW_KEY_I)
		{
			GetInstance()->m_pointLight->SetIntensity(GetInstance()->m_pointLight->GetIntensity() + 1.f);
		}
		else if (key == GLFW_KEY_J)
		{
			GetInstance()->m_pointLight->SetIntensity(GetInstance()->m_pointLight->GetIntensity() - 1.f);
		}
		// Control car speed
		else if (key == GLFW_KEY_M)
		{
			GetInstance()->m_carSpeed += .5f;
		}
		else if (key == GLFW_KEY_N)
		{
			GetInstance()->m_carSpeed -= .5f;
		}

		// Enable disco Light
		else if (key == GLFW_KEY_SPACE)
		{
			GetInstance()->m_discoModeOn = !GetInstance()->m_discoModeOn;
			for (int i = 0; i < DISCO_LIGHT_COUNT; i++)
			{
				GetInstance()->m_discoLights[i]->SetActive(!GetInstance()->m_discoModeOn);
			}
		}
	}
}

void MountainScene::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!s_IsMouseLeftButtonDown)
		return;

	static int wWidth, wHeight; // Window width and height

	// Get the center position of the window
	glfwGetWindowSize(window, &wWidth, &wHeight);
	static glm::vec2 center = glm::vec2(wWidth / 2, wHeight / 2);

	// Do not proceed if the position is the center of the window
	if (xpos == center.x && ypos == center.y)
		return;

	// Get Main Camera
	Camera* cam = Camera::GetMain();

	// The speed of the camera movement
	static float speed = .1f;

	// handle the mouse input
	glm::vec2 newPos = glm::vec2(xpos, ypos);
	glm::vec2 deltaPos = newPos - center;

	// Up down
	glm::vec3 cameraRotation = cam->GetTransform().GetRotation();
	cameraRotation += glm::vec3(deltaPos.y * speed * -1.f, .0f, .0f);

	// Left right
	cameraRotation += glm::vec3(.0f, deltaPos.x * speed * -1.f, .0f);

	// Clamp the rotation value
	if (cameraRotation.x >= 360.f || cameraRotation.x <= -360.f)
		cameraRotation.x = .0f;
	if (cameraRotation.y >= 360.f || cameraRotation.y <= -360.f)
		cameraRotation.y = .0f;

	// Set the rotation of the camera
	cam->GetTransform().SetRotation(cameraRotation);

	// Fix the cursor position to the center of the screen
	glfwSetCursorPos(window, wWidth / 2, wHeight / 2);
}

void MountainScene::mouse_button_callback(GLFWwindow* window, int button, int action, double xpos, int ypos)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			s_IsMouseLeftButtonDown = true;
		}
		else if (action == GLFW_RELEASE)
		{
			s_IsMouseLeftButtonDown = false;
		}
	}
}

void MountainScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

MountainScene* MountainScene::s_instance = nullptr;
bool MountainScene::s_IsMouseLeftButtonDown = false;

#pragma endregion

#pragma endregion

Shader* shader;
MountainScene* mountainScene;

void get_OpenGL_info()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

void sendDataToOpenGL()
{
	//TODO
	//Load objects and bind to VAO and VBO
	//Load textures
	Renderer::SetClearColor(glm::vec4(.5f, .7f, 1.f, 1.f));

	mountainScene = new MountainScene();
	SceneManager::SetActiveScene((Scene*)mountainScene);
}

void initializedGL(void) //run only once
{
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not OK." << std::endl;
	}

	get_OpenGL_info();
	sendDataToOpenGL();

	//TODO: set up the camera parameters	
	//TODO: set up the vertex shader and fragment shader
	shader = new Shader();
	shader->setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	shader->use();

	srand(time(NULL));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void paintGL(void)  //always run
{
	//TODO:
	//Set lighting information, such as position and color of lighting source
	//Set transformation matrix
	//Bind different textures
	Renderer::Clear();
	Camera::OnPaint(shader);
	ObjectRenderPipeline::OnPaint(shader);
	SceneManager::OnPaint(shader);
}

void onEnd(void)
{
	delete shader;
	delete mountainScene;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 2", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*register callback functions*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	Input::Init(window);

	initializedGL();

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	onEnd();

	glfwTerminate();

	return 0;
}