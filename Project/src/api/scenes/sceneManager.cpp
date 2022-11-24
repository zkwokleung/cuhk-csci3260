#include "scene.h"

std::list<Scene*> SceneManager::s_scenes;
Scene* SceneManager::s_activeScene = nullptr;
bool SceneManager::s_busying = false;

void SceneManager::OnInitialize()
{
	if (s_activeScene != nullptr)
	{
		s_activeScene->OnInitialize();
	}
}

void SceneManager::OnUpdate(void)
{
	if (s_busying)
	{
		return;
	}

	if (s_activeScene != nullptr)
	{
		s_activeScene->OnUpdate();
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

void SceneManager::SetActiveScene(int index)
{
	if (s_scenes.size() <= index)
	{
		std::stringstream msg;
		msg << "Scene of index \"" << index << "\" does not exist";
		Debug::LogWarning(msg.str());
		return;
	}
	std::list<Scene*>::iterator it = s_scenes.begin();
	std::advance(it, index);
	SetActiveScene(*it);
}

Scene* SceneManager::GetActiveScene()
{
	return s_activeScene;
}

void SceneManager::AddScene(Scene* scene)
{
	if (std::find(s_scenes.begin(), s_scenes.end(), scene) != s_scenes.end())
	{
		return;
	}

	s_scenes.push_back(scene);
}

void SceneManager::RemoveScene(Scene* scene)
{
	if (std::find(s_scenes.begin(), s_scenes.end(), scene) == s_scenes.end())
	{
		return;
	}

	s_scenes.remove(scene);
}
