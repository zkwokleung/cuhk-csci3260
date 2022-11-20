#pragma once
#include "collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	SphereCollider(float radius);
	~SphereCollider();

	float GetRadius() const;

private:
	float m_radius;
};