#pragma once
#include "../objects/object.h"
#include <list>
class Collider;

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

	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnUpdate();
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