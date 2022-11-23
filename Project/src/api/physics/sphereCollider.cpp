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
	if (IsAutoSize());
	{
		return glm::max(
			glm::max(
				glm::abs(GetObject()->GetTransform().GetScale().x),
				glm::abs(GetObject()->GetTransform().GetScale().y)
			),
			glm::abs(GetObject()->GetTransform().GetScale().z)
		);
	}
	return m_radius;
}
