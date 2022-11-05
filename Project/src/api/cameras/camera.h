#pragma once
#include "../renders/shader.h"
#include "../debugs/debug.h"
#include "../objects/transform.h"
#include "../objects/object.h"

class Camera : public Object
{
public:
	static Camera* GetMain();
	static void SetMain(Camera* camera);
	static void OnPaint(Shader* shader);

	Camera();
	~Camera();

	virtual glm::mat4 GetViewMatrix();
	virtual glm::mat4 GetProjectionMatrix();

private:
	static Camera* s_main;
};