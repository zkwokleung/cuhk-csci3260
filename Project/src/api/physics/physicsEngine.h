#pragma once
#include "collider.h"
#include <vector>

class PhysicsEngine
{
public:
	static void OnUpdate();

private:
	static void CheckCollisions();
	static void InitCollisionMap();
	static std::vector<Collider*> s_colliders;
	static std::vector<int*> s_collisionMap;
};