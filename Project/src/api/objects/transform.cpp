#include "Transform.h"

Transform::Transform()
	: m_localPosition(glm::vec3()), m_localRotation(glm::vec3()), m_localScale(glm::vec3(1.f)), m_parent(nullptr),
	m_childs()
{
}

Transform::~Transform()
{
}

glm::vec3 Transform::GetPosition() const
{
	return glm::vec3(GetTransformMat4()[3]);
}

void Transform::SetPosition(glm::vec3 value)
{
	if (m_parent != nullptr)
	{
		m_localPosition = value - m_parent->GetPosition();
	}
	else
	{
		m_localPosition = value;
	}
}

glm::vec3 Transform::GetRotation() const
{
	if (m_parent != nullptr)
	{
		return m_parent->GetRotation() + m_localRotation;
	}

	return m_localRotation;
}

void Transform::SetRotation(glm::vec3 value)
{
	if (m_parent != nullptr)
	{
		m_localRotation = value - m_parent->GetRotation();
	}
	else
	{
		m_localRotation = value;
	}
}

glm::vec3 Transform::GetScale() const
{
	if (m_parent != nullptr)
	{
		return glm::vec3(m_parent->GetScale().x * m_localScale.x, m_parent->GetScale().y * m_localScale.y,
			m_parent->GetScale().z * m_localScale.z);
	}

	return m_localScale;
}

void Transform::SetScale(glm::vec3 value)
{
	if (m_parent != nullptr)
	{
		m_localScale = glm::vec3(value.x / m_parent->GetScale().x, value.y / m_parent->GetScale().y,
			value.z / m_parent->GetScale().z);
	}
	else
	{
		m_localScale = value;
	}
}

void Transform::OnPaint(Shader* shader)
{
	glm::mat4 m = GetTransformMat4();
	shader->SetMat4("u_modelMatrix", m);

	glm::mat4 pos = GetPositionMat4();
	shader->SetMat4("u_positionMat4", pos);

	glm::mat4 rot = GetRotationMat4();
	shader->SetMat4("u_rotationMat4", rot);

	glm::mat4 s = GetScaleMat4();
	shader->SetMat4("u_scaleMat4", s);
}

glm::mat4 Transform::GetTransformMat4() const
{
	glm::mat4 model = glm::mat4(1.0f);

	// Translate
	model = glm::translate(model, GetLocalPosition());

	// Rotate
	model = glm::rotate(model, glm::radians(GetLocalRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(GetLocalRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(GetLocalRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));

	// Scale
	model = glm::scale(model, GetLocalScale());

	if (m_parent != nullptr)
	{
		return m_parent->GetTransformMat4() * model;
	}

	return model;
}

glm::mat4 Transform::GetRotationMat4() const
{
	glm::mat4 r = glm::mat4(1.0f);
	r = glm::rotate(r, glm::radians(GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
	r = glm::rotate(r, glm::radians(GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	r = glm::rotate(r, glm::radians(GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));

	if (m_parent != nullptr)
	{
		return m_parent->GetTransformMat4() * r;
	}
	return r;
}

glm::mat4 Transform::GetScaleMat4() const
{
	if (m_parent != nullptr)
	{
		return m_parent->GetScaleMat4() * glm::scale(glm::mat4(1.0f), GetScale());
	}
	return glm::scale(glm::mat4(1.0f), GetScale());
}

glm::mat4 Transform::GetPositionMat4() const
{
	if (m_parent != nullptr)
	{
		return m_parent->GetTransformMat4() * glm::translate(glm::mat4(1.f), GetPosition());
	}
	return glm::translate(glm::mat4(1.f), GetPosition());
}

glm::vec3 Transform::GetLocalPosition() const
{
	return m_localPosition;
}

void Transform::SetLocalPosition(glm::vec3 value)
{
	m_localPosition = value;
}

glm::vec3 Transform::GetLocalRotation() const
{
	return m_localRotation;
}

void Transform::SetLocalRotation(glm::vec3 value)
{
	m_localRotation = value;
	if (m_localRotation.x <= -360.f)
		m_localRotation.x += 360.f;
	else if (m_localRotation.x > 360.f)
		m_localRotation.x -= 360.f;

	if (m_localRotation.y <= -360.f)
		m_localRotation.y += 360.f;
	else if (m_localRotation.y > 360.f)
		m_localRotation.y -= 360.f;

	if (m_localRotation.z <= -360.f)
		m_localRotation.z += 360.f;
	else if (m_localRotation.z > 360.f)
		m_localRotation.z -= 360.f;
}

glm::vec3 Transform::GetLocalScale() const
{
	return m_localScale;
}

void Transform::SetLocalScale(glm::vec3 value)
{
	m_localScale = value;
}

glm::vec3 Transform::GetForward() const
{
	return -1.f * GetBackward();
}

glm::vec3 Transform::GetBackward() const
{
	return glm::vec3(glm::cos(glm::radians(GetRotation().x)) * glm::sin(glm::radians(GetRotation().y)),
		-1.f * glm::sin(glm::radians(GetRotation().x)),
		glm::cos(glm::radians(GetRotation().x)) * glm::cos(glm::radians(GetRotation().y)));
}

glm::vec3 Transform::GetRight() const
{
	return glm::vec3(glm::cos(glm::radians(GetRotation().y)),
		0,
		-1.f * glm::sin(glm::radians(GetRotation().y)));
}

glm::vec3 Transform::GetLeft() const
{
	return -1.f * GetRight();
}

glm::vec3 Transform::GetUp() const
{
	return glm::vec3(glm::sin(glm::radians(GetRotation().x)) * glm::sin(glm::radians(GetRotation().y)),
		glm::cos(glm::radians(GetRotation().x)),
		glm::sin(glm::radians(GetRotation().x)) * glm::cos(glm::radians(GetRotation().y))
	);
}

glm::vec3 Transform::GetDown() const
{
	return -1.f * GetUp();
}

Transform* Transform::GetParent()
{
	return m_parent;
}

void Transform::SetParent(Transform* transform)
{
	m_parent = transform;

	if (transform != nullptr)
	{
		transform->m_childs.push_back(this);
	}
}

std::list<Transform*> Transform::GetChilds() const
{
	return m_childs;
}

void Transform::Translate(glm::vec3 translation)
{
	SetLocalPosition(GetLocalPosition() + translation);
}

void Transform::Rotate(glm::vec3 rotation)
{
	SetLocalRotation(GetLocalRotation() + rotation);
}
