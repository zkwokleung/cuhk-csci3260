#include "scene.h"

Scene* SceneManager::s_activeScene = nullptr;
bool SceneManager::s_busying = false;

void SceneManager::OnInitialize()
{
	if (s_activeScene != nullptr)
	{
		s_activeScene->OnInitialize();
	}
}

void SceneManager::OnPaint(Shader* shader)
{
	if (s_busying)
	{
		return;
	}

	if (s_activeScene != nullptr)
	{
		s_activeScene->OnPaint(shader);
	}
}

void SceneManager::OnEnd()
{
	// End the current scene
	if (s_activeScene != nullptr)
	{
		s_activeScene->OnEnd();
	}
}

void SceneManager::SetActiveScene(Scene* scene)
{
	s_busying = true;

	// End the current scene
	if (s_activeScene != nullptr)
	{
		s_activeScene->OnEnd();
	}

	// Set the current scene
	s_activeScene = scene;

	// Initialize the current scene
	if (s_activeScene != nullptr)
	{
		s_activeScene->OnInitialize();
	}

	s_busying = false;
}

Scene* SceneManager::GetActiveScene()
{
	return s_activeScene;
}

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