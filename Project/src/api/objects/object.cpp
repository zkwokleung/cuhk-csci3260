#include "object.h"

Object::Object() :
	m_transform(), m_isActive(false), m_name("")
{
}

Object::Object(std::string name) : m_transform(), m_isActive(false), m_name(name)
{
}

Object::~Object()
{
}

Transform& Object::GetTransform()
{
	return m_transform;
}

void Object::SetActive(bool active)
{
	if (active)
	{
		ObjectRenderPipeline::AddObject(this);
	}
	else
	{
		ObjectRenderPipeline::RemoveObject(this);
	}

	m_isActive = active;
}

bool Object::IsActive() const
{
	return m_isActive;
}

void Object::OnPaint(Shader* shader)
{
	m_transform.OnPaint(shader);
}

void Object::SetName(std::string name)
{
	m_name = name;
}

std::string Object::GetName() const
{
	return m_name;
}

std::list<Object*> ObjectRenderPipeline::s_objects;

void ObjectRenderPipeline::AddObject(Object* object)
{
	if (object->IsActive())
		return;
	s_objects.push_back(object);
}

void ObjectRenderPipeline::RemoveObject(Object* object)
{
	if (!object->IsActive())
		return;
	s_objects.remove(object);
}

void ObjectRenderPipeline::OnPaint(Shader* shader)
{
	if (s_objects.size() < 1)
		return;

	std::list<Object*>::iterator it;
	for (it = s_objects.begin(); it != s_objects.end(); it++)
	{
		(*it)->OnPaint(shader);
	}
}