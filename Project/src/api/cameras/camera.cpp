#include "Camera.h"

Camera* Camera::s_main = nullptr;

Camera* Camera::GetMain()
{
	return s_main;
}

void Camera::SetMain(Camera* camera)
{
	s_main = camera;
}

void Camera::Update(void)
{
	if (s_main == nullptr)
	{
		return;
	}

	s_main->OnUpdate();
}

void Camera::Paint(Shader* shader)
{
	if (s_main == nullptr)
	{
		Debug::LogError("No Main Camera Set");
		return;
	}

	// Set the shader's projection and view uniform
	glm::mat4 v = s_main->GetViewMatrix();
	glm::mat4 p = s_main->GetProjectionMatrix();
	shader->SetMat4("u_viewMatrix", v);
	shader->SetMat4("u_projectionMatrix", p);
	shader->SetVec3("u_viewPos", GetMain()->GetTransform().GetPosition());
}

Camera::Camera() : Object()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewRotationMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	// Rotate
	view = glm::rotate(view, glm::radians(GetTransform().GetRotation().x * -1.f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(GetTransform().GetRotation().y * -1.f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, glm::radians(GetTransform().GetRotation().z * -1.f), glm::vec3(0.0f, 0.0f, 1.0f));

	return view;
}

glm::mat4 Camera::GetViewTranslationMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	// Translate
	view = glm::translate(view, GetTransform().GetPosition() * -1.f);
	return view;
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);

	// Rotate
	view = glm::rotate(view, glm::radians(GetTransform().GetRotation().x * -1.f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(GetTransform().GetRotation().y * -1.f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, glm::radians(GetTransform().GetRotation().z * -1.f), glm::vec3(0.0f, 0.0f, 1.0f));

	// Translate
	view = glm::translate(view, GetTransform().GetPosition() * -1.f);

	return view;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100000.0f);
}