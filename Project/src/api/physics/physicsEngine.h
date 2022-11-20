#pragma once
#include "collider.h"
#include "boxCollider.h"
#include "sphereCollider.h"
#include <vector>

class PhysicsEngine
{
public:
	static void OnUpdate();

	static bool IsColliding(BoxCollider* a, BoxCollider* b);
	static bool IsColliding(SphereCollider* sphere, SphereCollider* other);
	static bool IsColliding(SphereCollider* sphere, BoxCollider* box);
	static bool IsColliding(BoxCollider* box, SphereCollider* sphere);
	// Check the type of the colliders and map them to the corresponding collision checking function
	static bool IsColliding(Collider* a, Collider* b);

private:
	// Check the collisions for all active colliders
	static void CheckCollisions();
	static void InitCollisionMap();
	static std::vector<Collider*> s_colliders;
	static std::vector<int*> s_collisionMap;
};