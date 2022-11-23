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
	if (IsAutoSize())
	{
		return GetObject()->GetTransform().GetPosition().x - GetObject()->GetTransform().GetScale().x;
	}
	return GetObject()->GetTransform().GetPosition().x - (m_size.x / 2.f);
}

float BoxCollider::GetMaxX()
{
	if (IsAutoSize())
	{
		return GetObject()->GetTransform().GetPosition().x + GetObject()->GetTransform().GetScale().x;
	}
	return GetObject()->GetTransform().GetPosition().x + (m_size.x / 2.f);
}

float BoxCollider::GetMinY()
{
	if (IsAutoSize())
	{
		return GetObject()->GetTransform().GetPosition().y - GetObject()->GetTransform().GetScale().y;
	}
	return GetObject()->GetTransform().GetPosition().y - (m_size.y / 2.f);
}

float BoxCollider::GetMaxY()
{
	if (IsAutoSize())
	{
		return GetObject()->GetTransform().GetPosition().y + GetObject()->GetTransform().GetScale().y;
	}
	return GetObject()->GetTransform().GetPosition().y + (m_size.y / 2.f);
}

float BoxCollider::GetMinZ()
{
	if (IsAutoSize())
	{
		return GetObject()->GetTransform().GetPosition().z - GetObject()->GetTransform().GetScale().z;
	}
	return GetObject()->GetTransform().GetPosition().z - (m_size.z / 2.f);
}

float BoxCollider::GetMaxZ()
{
	if (IsAutoSize())
	{
		return GetObject()->GetTransform().GetPosition().z + GetObject()->GetTransform().GetScale().z;
	}
	return GetObject()->GetTransform().GetPosition().z + (m_size.z / 2.f);
}
