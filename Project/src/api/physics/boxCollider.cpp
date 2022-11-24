#include "physicsEngine.h"

BoxCollider::BoxCollider() : Collider(), m_size(glm::vec3(1.f))
{
}

BoxCollider::BoxCollider(glm::vec3 size) : Collider(), m_size(size)
{
}

BoxCollider::~BoxCollider()
{
}

float BoxCollider::GetMinX()
{
	return GetObject()->GetTransform().GetPosition().x - (m_size.x / 2.f);
}

float BoxCollider::GetMaxX()
{
	return GetObject()->GetTransform().GetPosition().x + (m_size.x / 2.f);
}

float BoxCollider::GetMinY()
{
	return GetObject()->GetTransform().GetPosition().y - (m_size.y / 2.f);
}

float BoxCollider::GetMaxY()
{
	return GetObject()->GetTransform().GetPosition().y + (m_size.y / 2.f);
}

float BoxCollider::GetMinZ()
{
	return GetObject()->GetTransform().GetPosition().z - (m_size.z / 2.f);
}

float BoxCollider::GetMaxZ()
{
	return GetObject()->GetTransform().GetPosition().z + (m_size.z / 2.f);
}
