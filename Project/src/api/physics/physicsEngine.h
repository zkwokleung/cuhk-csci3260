#pragma once
#include "collider.h"
#include "boxCollider.h"
#include "sphereCollider.h"
#include <vector>

class PhysicsEngine
{
public:
	static void Init(void);
	static void OnUpdate(void);

	// AABB vs AABB
	static bool IsColliding(BoxCollider* a, BoxCollider* b);
	// Sphere vs Sphere
	static bool IsColliding(SphereCollider* sphere, SphereCollider* other);
	// Sphere vs AABB
	static bool IsColliding(SphereCollider* sphere, BoxCollider* box);
	// AABB vs Sphere
	static bool IsColliding(BoxCollider* box, SphereCollider* sphere);
	// Check the type of the colliders and map them to the corresponding collision checking function
	static bool IsColliding(Collider* a, Collider* b);

	static void AddCollider(Collider* collider);
	static void RemoveCollider(Collider* collider);

private:
	// Check the collisions for all active colliders
	static void CheckCollisions();
	static std::vector<Collider*> s_colliders;
};