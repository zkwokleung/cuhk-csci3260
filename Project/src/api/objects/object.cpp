#include "object.h"

Object::Object() :
	m_transform(), m_isActive(false), m_name(""), m_components()
{
	ObjectRenderPipeline::AddObject(this);
}

Object::Object(std::string name) : m_transform(), m_isActive(false), m_name(name), m_components()
{
	ObjectRenderPipeline::AddObject(this);
}

Object::~Object()
{
	// Delete all components
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		delete m_components.front();
		m_components.pop_front();
	}
	ObjectRenderPipeline::RemoveObject(this);
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
		if (*it != nullptr && (*it)->Enabled())
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
std::list<Object*> ObjectRenderPipeline::s_renderPipeline;
std::list<Object*> ObjectRenderPipeline::s_toBeRemoved;
bool ObjectRenderPipeline::s_updating = false;

void ObjectRenderPipeline::AddObject(Object* object)
{
	if (std::find(s_objects.begin(), s_objects.end(), object) != s_objects.end())
		return;
	s_objects.push_back(object);
}

void ObjectRenderPipeline::RemoveObject(Object* object)
{
	// If it is in the update loop, store the object first and remove it on the next loop
	if (s_updating)
	{
		s_toBeRemoved.push_back(object);
		return;
	}

	if (std::find(s_objects.begin(), s_objects.end(), object) == s_objects.end())
		return;
	s_objects.remove(object);
}

void ObjectRenderPipeline::OnUpdate(void)
{
	if (s_objects.size() < 1)
		return;

	s_renderPipeline = std::list<Object*>(s_objects);

	// Check if there are anything to remove
	for (std::list<Object*>::iterator it = s_toBeRemoved.begin(); s_toBeRemoved.size() != 0; )
	{
		if (*it != nullptr && (*it)->IsActive())
		{
			RemoveObject(*it);
			s_toBeRemoved.remove(*it);
		}
	}

	// Start the update loop
	s_updating = true;
	for (std::list<Object*>::iterator it = s_renderPipeline.begin(); it != s_renderPipeline.end(); it++)
	{
		if (*it != nullptr && (*it)->IsActive())
		{
			(*it)->OnUpdate();
		}
	}
	s_updating = false;
}

void ObjectRenderPipeline::OnPaint(Shader* shader)
{
	if (s_objects.size() < 1)
		return;

	std::list<Object*>::iterator it;
	for (it = s_renderPipeline.begin(); it != s_renderPipeline.end(); it++)
	{
		if (*it != nullptr && (*it)->IsActive())
		{
			(*it)->OnPaint(shader);
		}
	}
}