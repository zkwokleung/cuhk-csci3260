#pragma once
#include <list>
#include "objects.h"
#include "shader.h"

class Scene
{
public:
	Scene();
	~Scene();

	virtual void OnInitialize();
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
	static void OnInitialize();
	static void OnPaint(Shader* shader);
	static void OnEnd();
	static void SetActiveScene(Scene* scene);
	static Scene* GetActiveScene();

private:
	static Scene* s_activeScene;
	static bool s_busying;
};