#pragma once
#include "../objects/component.h"
#include <list>

class ICollisionCallback
{
public:
	virtual void OnCollision(Collider* self, Collider* other) = 0;
private:
	friend class Collider;
};

class Collider : public Component
{
public:
	Collider();
	~Collider();

	virtual void OnPaint(Shader* shader);
	virtual bool IsCollidingWith(Collider* other);

	void AddCollisionCallback(ICollisionCallback* callback);
	void RemoveCollisionCallback(ICollisionCallback* callback);

protected:
	friend class PhysicsEngine;

private:
	void OnCollide(Collider* other);
	std::list<ICollisionCallback*> m_callbacks;
};