#include "collider.h"

Collider::Collider() : Object()
{
}

Collider::~Collider()
{
}

void Collider::OnPaint(Shader* shader)
{
}

bool Collider::IsCollidingWith(Collider other)
{
	return false;
}

void Collider::SetCollisionCallback(CollisionCallbackFunc func)
{
}

void Collider::OnCollide(Collider other)
{
}
