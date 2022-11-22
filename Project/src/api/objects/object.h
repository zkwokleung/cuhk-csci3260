#pragma once
#include "../renderModule.h"
#include "transform.h"

class Component;
class Object;

class Component
{
public:
	Component();
	~Component();

	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnUpdate();
	virtual void OnPaint(Shader* shader);

	bool Enabled() const;
	void SetEnabled(bool enabled);

	Object* GetObject() const;

private:
	friend class Object;

	Object* m_object;
	bool m_enabled;
};

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