#include "component.h"

Component::Component(): m_object(nullptr)
{
}

Component::~Component()
{
}

void Component::OnUpdate()
{
}

void Component::OnPaint(Shader* shader)
{
}

Object* Component::GetObject() const
{
	return m_object;
}
