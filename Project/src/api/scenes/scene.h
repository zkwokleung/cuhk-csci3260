#pragma once
#include <list>
#include <iterator>
#include "../objects/object.h"
#include "../renders/shader.h"
#include "../debugModule.h"

class Scene
{
public:
	Scene();
	~Scene();

	virtual void OnInitialize();
	virtual void OnUpdate(void);
	virtual void OnPaint(Shader* shader);
	virtual void OnEnd();

	void AddObject(Object* object);
	void RemoveObject(Object* object);
	std::list<Object*> GetObjects() const;

protected:
	std::list<Object*> m_objects;
	bool m_initialized;
};

class SceneManager
{
public:
	// Initialize the Scene Manager
	static void OnInitialize();
	// OnUpdate is called on every loop
	static void OnUpdate(void);
	// OnPaint is called on every frame
	static void OnPaint(Shader* shader);
	// OnEnd is called when a scene end
	static void OnEnd();
	// Set the current active scene
	static void SetActiveScene(Scene* scene);
	// Set the current active scene by scene index
	static void SetActiveScene(int index);
	// Get the current active scene
	static Scene* GetActiveScene();
	// Add a scene to the scene list
	static void AddScene(Scene* scene);
	// Remove a scene from the scene list
	static void RemoveScene(Scene* scene);

private:
	static std::list<Scene*> s_scenes;
	static Scene* s_activeScene;
	static bool s_busying;
	static Scene* s_nextScene;
};