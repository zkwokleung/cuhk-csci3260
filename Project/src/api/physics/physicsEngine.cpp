#include "physicsEngine.h"

std::vector<Collider*> PhysicsEngine::s_colliders;

void PhysicsEngine::Init(void)
{
}

void PhysicsEngine::OnUpdate()
{
	CheckCollisions();
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
		(sphere->GetObject()->GetTransform().GetPosition().x - other->GetObject()->GetTransform().GetPosition().x) * (sphere->GetObject()->GetTransform().GetPosition().x - other->GetObject()->GetTransform().GetPosition().x) +
		(sphere->GetObject()->GetTransform().GetPosition().y - other->GetObject()->GetTransform().GetPosition().y) * (sphere->GetObject()->GetTransform().GetPosition().y - other->GetObject()->GetTransform().GetPosition().y) +
		(sphere->GetObject()->GetTransform().GetPosition().z - other->GetObject()->GetTransform().GetPosition().z) * (sphere->GetObject()->GetTransform().GetPosition().z - other->GetObject()->GetTransform().GetPosition().z)
	);

	return distance < (sphere->GetRadius() + other->GetRadius());
}

bool PhysicsEngine::IsColliding(SphereCollider* sphere, BoxCollider* box)
{
	float x = glm::max(box->GetMinX(), glm::min(sphere->GetObject()->GetTransform().GetPosition().x, box->GetMaxX()));
	float y = glm::max(box->GetMinY(), glm::min(sphere->GetObject()->GetTransform().GetPosition().y, box->GetMaxY()));
	float z = glm::max(box->GetMinZ(), glm::min(sphere->GetObject()->GetTransform().GetPosition().z, box->GetMaxZ()));

	float distance = glm::sqrt(
		(x - sphere->GetObject()->GetTransform().GetPosition().x) * (x - sphere->GetObject()->GetTransform().GetPosition().x) +
		(y - sphere->GetObject()->GetTransform().GetPosition().y) * (y - sphere->GetObject()->GetTransform().GetPosition().y) +
		(z - sphere->GetObject()->GetTransform().GetPosition().z) * (z - sphere->GetObject()->GetTransform().GetPosition().z)
	);

	return distance < sphere->GetRadius();
}

bool PhysicsEngine::IsColliding(BoxCollider* box, SphereCollider* sphere)
{
	return IsColliding(sphere, box);
}

void PhysicsEngine::CheckCollisions()
{
	InitCollisionMap();

	// Check collision for all colliders
	for (int i = 0; i < s_colliders.size(); i++)
	{
		for (int j = i + 1; j < s_colliders.size(); j++)
		{
			if (IsColliding(s_colliders[i], s_colliders[j]))
			{
				// Invoke the collision callback
				s_colliders[i]->OnCollide(s_colliders[j]);
				s_colliders[j]->OnCollide(s_colliders[i]);
			}
		}
	}
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

void PhysicsEngine::AddCollider(Collider* collider)
{
	if (std::find(s_colliders.begin(), s_colliders.end(), collider) != s_colliders.end())
	{
		return;
	}

	s_colliders.push_back(collider);
}

void PhysicsEngine::RemoveCollider(Collider* collider)
{
	if (std::find(s_colliders.begin(), s_colliders.end(), collider) == s_colliders.end())
	{
		return;
	}

	s_colliders.erase(std::remove(s_colliders.begin(), s_colliders.end(), collider), s_colliders.end());
}
