#include "scene.h"

Scene::Scene() : m_objects(), m_initialized(false)
{
}

Scene::~Scene()
{
	while (m_objects.size() > 0)
	{
		Object* o = *(m_objects.begin());
		m_objects.remove(o);
		delete o;
	}
}

void Scene::OnInitialize()
{
	if (m_initialized)
		return;

	m_initialized = true;
}

void Scene::OnPaint(Shader* shader)
{
}

void Scene::OnEnd()
{
}

void Scene::AddObject(Object* object)
{
	m_objects.push_back(object);
}

void Scene::RemoveObject(Object* object)
{
	m_objects.remove(object);
}

std::list<Object*> Scene::GetObjects() const
{
	return m_objects;
}