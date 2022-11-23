#include "object.h"

Component::Component() : m_object(nullptr), m_enabled(false)
{
}

Component::~Component()
{
}

void Component::OnEnable()
{
}

void Component::OnDisable()
{
}

void Component::OnUpdate()
{
}

void Component::OnPaint(Shader* shader)
{
}

bool Component::Enabled() const
{
	return m_enabled;
}

void Component::SetEnabled(bool enabled)
{
	m_enabled = enabled;

	if (m_enabled)
	{
		OnEnable();
	}
	else
	{
		OnDisable();
	}
}

Object* Component::GetObject() const
{
	return m_object;
}