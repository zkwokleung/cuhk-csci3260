#include "physicsEngine.h"

Collider::Collider() : Component(), m_callbacks()
{
}

Collider::~Collider()
{
}

void Collider::OnEnable()
{
	// Add to physics engine's collider list
	PhysicsEngine::AddCollider(this);
}

void Collider::OnDisable()
{
	// Remove from physics engine's list
	PhysicsEngine::RemoveCollider(this);
}

void Collider::OnUpdate()
{
}

void Collider::OnPaint(Shader* shader)
{
}

bool Collider::IsCollidingWith(Collider* other)
{
	return false;
}

void Collider::AddCollisionCallback(ICollisionCallback* callback)
{
	if ((std::find(m_callbacks.begin(), m_callbacks.end(), callback) != m_callbacks.end()))
	{
		return;
	}

	m_callbacks.push_back(callback);
}

void Collider::RemoveCollisionCallback(ICollisionCallback* callback)
{
	if ((std::find(m_callbacks.begin(), m_callbacks.end(), callback) == m_callbacks.end()))
	{
		return;
	}

	m_callbacks.remove(callback);
}

void Collider::OnCollide(Collider* other)
{
	// Invoke all listener
	std::list<ICollisionCallback*>::iterator it;
	for (it = m_callbacks.begin(); it != m_callbacks.end(); it++)
	{
		(*it)->OnCollision(this, other);
	}
}