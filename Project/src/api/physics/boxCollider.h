#pragma once
#include "collider.h"

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