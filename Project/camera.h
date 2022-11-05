#pragma once
#include "Shader.h"
#include "Transform.h"
#include "Debug.h"

class Camera
{
public:
	static Camera* GetMain();
	static void SetMain(Camera* camera);
	static void OnPaint(Shader* shader);

	Camera();
	~Camera();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	Transform& GetTransform();
private:
	static Camera* s_main;

	Transform m_transform;
};