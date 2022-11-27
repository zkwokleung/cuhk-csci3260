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

void SphereCollider::SetRadius(float radius)
{
	m_radius = radius;
}

float SphereCollider::GetRadius() const
{
	return m_radius;
}
