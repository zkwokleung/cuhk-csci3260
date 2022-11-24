#include "object.h"

Object::Object() :
	m_transform(), m_isActive(false), m_name(""), m_components()
{
}

Object::Object(std::string name) : m_transform(), m_isActive(false), m_name(name), m_components()
{
}

Object::~Object()
{
	// Delete all components
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		delete m_components.front();
		m_components.pop_front();
	}
}

Transform& Object::GetTransform()
{
	return m_transform;
}

void Object::SetActive(bool active)
{
	m_isActive = active;
	if (active)
	{
		ObjectRenderPipeline::AddObject(this);
		OnEnable();
	}
	else
	{
		ObjectRenderPipeline::RemoveObject(this);
		OnDisable();
	}
}

bool Object::IsActive() const
{
	return m_isActive;
}

void Object::OnEnable(void)
{
	// Enable all components
	//std::list<Component*>::iterator it;
	//for (it = m_components.begin(); it != m_components.end(); it++)
	//{
	//	(*it)->SetEnabled(true);
	//}
}

void Object::OnDisable(void)
{
	// Disable all components
	//std::list<Component*>::iterator it;
	//for (it = m_components.begin(); it != m_components.end(); it++)
	//{
	//	(*it)->SetEnabled(false);
	//}
}

void Object::OnUpdate(void)
{
	std::list<Component*>::iterator it;
	for (it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->OnUpdate();
	}
}

void Object::OnPaint(Shader* shader)
{
	m_transform.OnPaint(shader);

	std::list<Component*>::iterator it;
	for (it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->OnPaint(shader);
	}
}

void Object::SetName(std::string name)
{
	m_name = name;
}

std::string Object::GetName() const
{
	return m_name;
}

void Object::AddComponent(Component* component)
{
	if (component->m_object != nullptr)
	{
		return;
	}

	m_components.push_back(component);
	component->m_object = this;
}

void Object::RemoveComponent(Component* component)
{
	if (component->m_object != this)
	{
		return;
	}

	m_components.remove(component);
	component->m_object = nullptr;
}

std::list<Object*> ObjectRenderPipeline::s_objects;

void ObjectRenderPipeline::AddObject(Object* object)
{
	if (std::find(s_objects.begin(), s_objects.end(), object) != s_objects.end())
		return;
	s_objects.push_back(object);
}

void ObjectRenderPipeline::RemoveObject(Object* object)
{
	if (std::find(s_objects.begin(), s_objects.end(), object) == s_objects.end())
		return;
	s_objects.remove(object);
}

void ObjectRenderPipeline::OnUpdate(void)
{
	if (s_objects.size() < 1)
		return;

	std::list<Object*>::iterator it;
	for (it = s_objects.begin(); it != s_objects.end(); it++)
	{
		(*it)->OnUpdate();
	}
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