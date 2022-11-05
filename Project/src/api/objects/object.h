#pragma once
#include "../renderModule.h"
#include "transform.h"

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

	virtual void OnPaint(Shader* shader);

	void SetName(std::string name);
	std::string GetName() const;

protected:
	std::string m_name;
	Transform m_transform;
	bool m_isActive;
};

class ObjectRenderPipeline
{
public:
	static void AddObject(Object* object);
	static void RemoveObject(Object* object);
	static void OnPaint(Shader* shader);
private:
	static std::list<Object*> s_objects;
};