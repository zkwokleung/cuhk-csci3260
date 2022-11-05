#pragma once
#include "../objects/object.h"

typedef void(*CollisionCallbackFunc)(Collider other);

class Collider : public Object
{
public:
	Collider();
	~Collider();

	virtual void OnPaint(Shader* shader);

	virtual bool IsCollidingWith(Collider other);
	void SetCollisionCallback(CollisionCallbackFunc func);

private:
	void OnCollide(Collider other);
	CollisionCallbackFunc m_callback;
};