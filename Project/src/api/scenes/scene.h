#pragma once
#include <list>
#include "../objects/object.h"
#include "../renders/shader.h"

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