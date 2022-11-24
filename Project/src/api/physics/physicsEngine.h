#pragma once
#include "../objectModule.h"
#include <vector>

class Collider;
class BoxCollider;
class SphereCollider;
class PhysicsEngine;

class ICollisionCallback
{
public:
	virtual void OnCollision(Collider* self, Collider* other) = 0;
private:
	friend class Collider;
};

class Collider : public Component
{
public:
	Collider();
	~Collider();

	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnUpdate();
	virtual void OnPaint(Shader* shader);
	virtual bool IsCollidingWith(Collider* other);

	void AddCollisionCallback(ICollisionCallback* callback);
	void RemoveCollisionCallback(ICollisionCallback* callback);

protected:
	friend class PhysicsEngine;

private:
	void OnCollide(Collider* other);
	std::list<ICollisionCallback*> m_callbacks;
};

class SphereCollider : public Collider
{
public:
	SphereCollider();
	SphereCollider(float radius);
	~SphereCollider();

	float GetRadius() const;

private:
	float m_radius;
};

class BoxCollider : public Collider
{
public:
	BoxCollider();
	BoxCollider(glm::vec3 size);
	~BoxCollider();

	float GetMinX();
	float GetMaxX();
	float GetMinY();
	float GetMaxY();
	float GetMinZ();
	float GetMaxZ();

private:
	glm::vec3 m_size;
};

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