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

	virtual void OnEnable(void);
	virtual void OnDisable(void);
	virtual void OnUpdate(void);
	virtual void OnPaint(Shader* shader);

	void SetName(std::string name);
	std::string GetName() const;

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	// Get the first component found of type T
	template<class T>
	T* GetComponent(void);

protected:
	std::string m_name;
	Transform m_transform;
	bool m_isActive;
	std::list<Component*> m_components;
};

template<class T>
inline T* Object::GetComponent(void)
{
	for (std::list<Component*>::iterator it = m_components.begin(); it != m_components.end(); it++)
	{
		if (dynamic_cast<T*>(*it) != nullptr)
		{
			return (T*)(*it);
		}
	}
	std::stringstream msg;
	msg << "No compoent of type \"" << typeid(T).name() << "\" found.";
	Debug::LogWarning(msg.str());
	return nullptr;
}

class ObjectRenderPipeline
{
public:
	static void AddObject(Object* object);
	static void RemoveObject(Object* object);
	static void OnUpdate(void);
	static void OnPaint(Shader* shader);
private:
	static std::list<Object*> s_objects;
	// Store the objects that are to be removed and perform the removal on the next update loop
	static std::list<Object*> s_toBeRemoved;
	static bool s_updating;
};