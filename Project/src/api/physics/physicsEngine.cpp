#include "physicsEngine.h"

std::vector<Collider*> PhysicsEngine::s_colliders;

void PhysicsEngine::OnUpdate()
{
}

bool PhysicsEngine::IsColliding(BoxCollider* a, BoxCollider* b)
{
	return (
		a->GetMinX() <= b->GetMaxX() &&
		a->GetMaxX() >= b->GetMinX() &&
		a->GetMinY() <= b->GetMaxY() &&
		a->GetMaxY() >= b->GetMinY() &&
		a->GetMinZ() <= b->GetMaxZ() &&
		a->GetMaxZ() >= b->GetMinZ()
		);
}

bool PhysicsEngine::IsColliding(SphereCollider* sphere, SphereCollider* other)
{
	float distance = glm::sqrt(
		(sphere->GetTransform().GetPosition().x - other->GetTransform().GetPosition().x) * (sphere->GetTransform().GetPosition().x - other->GetTransform().GetPosition().x) +
		(sphere->GetTransform().GetPosition().y - other->GetTransform().GetPosition().y) * (sphere->GetTransform().GetPosition().y - other->GetTransform().GetPosition().y) +
		(sphere->GetTransform().GetPosition().z - other->GetTransform().GetPosition().z) * (sphere->GetTransform().GetPosition().z - other->GetTransform().GetPosition().z)
	);

	return distance < (sphere->GetRadius() + other->GetRadius());
}

bool PhysicsEngine::IsColliding(SphereCollider* sphere, BoxCollider* box)
{
	float x = glm::max(box->GetMinX(), glm::min(sphere->GetTransform().GetPosition().x, box->GetMaxX()));
	float y = glm::max(box->GetMinY(), glm::min(sphere->GetTransform().GetPosition().y, box->GetMaxY()));
	float z = glm::max(box->GetMinZ(), glm::min(sphere->GetTransform().GetPosition().z, box->GetMaxZ()));

	float distance = glm::sqrt(
		(x - sphere->GetTransform().GetPosition().x) * (x - sphere->GetTransform().GetPosition().x) +
		(y - sphere->GetTransform().GetPosition().y) * (y - sphere->GetTransform().GetPosition().y) +
		(z - sphere->GetTransform().GetPosition().z) * (z - sphere->GetTransform().GetPosition().z)
	);

	return distance < sphere->GetRadius();
}

bool PhysicsEngine::IsColliding(BoxCollider* box, SphereCollider* sphere)
{
	return IsColliding(sphere, box);
}

void PhysicsEngine::CheckCollisions()
{
}

bool PhysicsEngine::IsColliding(Collider* a, Collider* b)
{
	if (typeid(*a) == typeid(BoxCollider))
	{
		// a is a box collider
		if (typeid(*b) == typeid(BoxCollider))
		{
			// b is a box collider
			return IsColliding((BoxCollider*)a, (BoxCollider*)b);
		}
		else if (typeid(*b) == typeid(SphereCollider))
		{
			// b is a sphere collider
			return IsColliding((BoxCollider*)a, (SphereCollider*)b);
		}
	}
	else if (typeid(*a) == typeid(SphereCollider))
	{
		// a is a sphere collider
		if (typeid(*b) == typeid(BoxCollider))
		{
			// b is a box collider
			return IsColliding((SphereCollider*)a, (BoxCollider*)b);
		}
		else if (typeid(*b) == typeid(SphereCollider))
		{
			// b is a sphere collider
			return IsColliding((SphereCollider*)a, (SphereCollider*)b);
		}
	}

	// Collider type unknown
	return false;
}
