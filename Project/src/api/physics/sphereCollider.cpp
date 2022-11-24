#include "physicsEngine.h"

SphereCollider::SphereCollider() : Collider(), m_radius(1.f)
{
}

SphereCollider::SphereCollider(float radius) : Collider(), m_radius(radius)
{
}

SphereCollider::~SphereCollider()
{
}

float SphereCollider::GetRadius() const
{
	return m_radius;
}
