#pragma once
#include "../renders/shader.h"
#include "object.h"

class Component
{
public:
	Component();
	~Component();

	virtual void OnUpdate();
	virtual void OnPaint(Shader* shader);

	Object* GetObject() const;

private:
	friend class Object;

	Object* m_object;
};