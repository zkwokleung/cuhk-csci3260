#include "perspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera() : Camera(),
m_fovy(glm::radians(45.0f)), m_aspect(1.f), m_near(.1f), m_far(100000.f)
{
}

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far) : Camera(),
m_fovy(fovy), m_aspect(aspect), m_near(near), m_far(far)
{
}

PerspectiveCamera::~PerspectiveCamera()
{
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix()
{
	return glm::perspective(m_fovy, m_aspect, m_near, m_far);
}

void PerspectiveCamera::SetFovy(float fovy)
{
	m_fovy = fovy;
}

float PerspectiveCamera::GetFovy() const
{
	return m_fovy;
}

void PerspectiveCamera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

float PerspectiveCamera::GetAspect() const
{
	return m_aspect;
}

void PerspectiveCamera::SetNear(float zNear)
{
	m_near = zNear;
}

float PerspectiveCamera::GetNear() const
{
	return m_near;
}

void PerspectiveCamera::SetFar(float zFar)
{
	m_far = zFar;
}

float PerspectiveCamera::GetFar() const
{
	return m_far;
}
