#pragma once
#include "../renderModule.h"
#include "transform.h"
#include "component.h"

#define DEFAULT_OBJECT_COLOR_VALUE 0.8f, 0.8f, 0.8f, 1.0f

class Object
{
public:
	Object();
	Object(std::string name);
	~Object();

	Transform& GetTransform();

	virtual void SetActive(bool active);
	virtual bool IsActive() const;

	virtual void OnUpdate(void);
	virtual void OnPaint(Shader* shader);

	void SetName(std::string name);
	std::string GetName() const;

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

protected:
	std::string m_name;
	Transform m_transform;
	bool m_isActive;
	std::list<Component*> m_components;
};

class ObjectRenderPipeline
{
public:
	static void AddObject(Object* object);
	static void RemoveObject(Object* object);
	static void OnUpdate(void);
	static void OnPaint(Shader* shader);
private:
	static std::list<Object*> s_objects;
};