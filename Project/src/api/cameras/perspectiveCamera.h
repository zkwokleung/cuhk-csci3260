#pragma once
#include "camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera();
	PerspectiveCamera(float fovy, float aspect, float near, float far);
	~PerspectiveCamera();

	virtual glm::mat4 GetProjectionMatrix();

	void SetFovy(float fovy);
	float GetFovy() const;
	void SetAspect(float aspect);
	float GetAspect() const;
	void SetNear(float zNear);
	float GetNear() const;
	void SetFar(float zFar);
	float GetFar() const;

private:
	float m_fovy, m_aspect, m_near, m_far;
};