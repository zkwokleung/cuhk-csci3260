/*
Type your name and student ID here
	- Name: SZE-TO Kwok Leung
	- Student ID: 1155149068
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <list>

GLint programID;

#pragma region My API
/***************************************************************
	Classes and functions created by me to simplify some codes
****************************************************************/

/**********************
	Render related
***********************/
class VBO
{
public:
	GLuint ID;
	VBO();
	VBO(const GLfloat* vertices, GLsizeiptr size);
	~VBO();

	// Bind the VBO to allow OpenGL to use it
	void Bind() const;
	void Unbind() const;
	void Delete();
};

class VAO
{
public:
	GLuint ID; // The ID of the VAO
	VAO();
	~VAO();

	// Linking the buffer to this array
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint componentSize, GLenum type, GLsizei stride, void* offset);

	// Bind the VAO to allow OpenGL to use it
	void Bind() const;
	void Unbind() const;
	void Delete();
};

class EBO
{
public:
	GLuint ID; // The ID of the EBO
	GLuint Count; // The number of indecies

	EBO();
	EBO(const GLuint* indices, GLuint count);
	~EBO();

	void Bind() const;
	void Unbind() const;
	void Delete();
};

// The Renderer is responsible for drawing things to the window.
class Renderer
{
public:
	// Clear everything that is currently displayed
	static void Clear();
	// Draw an element with the given vertices and the specific shape
	static void Draw(const VAO& vao, GLenum mode, int vertexCount);
	// Draw an element with the given vertices array and indices array
	static void Draw(const VAO& vao, const EBO& ebo);
private:

};

VBO::VBO() : ID(0) {
}

VBO::VBO(const GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	Unbind();
}

VBO::~VBO()
{
	Delete();
}

void VBO::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	if (ID != 0)
	{
		glDeleteBuffers(1, &ID);
	}
}

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

VAO::~VAO()
{
	Delete();
}


void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint componentSize, GLenum type, GLsizei stride, void* offset)
{
	Bind();
	VBO.Bind();
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, componentSize, type, GL_FALSE, stride, offset);
	VBO.Unbind();
	Unbind();
}

void VAO::Bind() const
{
	glBindVertexArray(ID);
}

void VAO::Unbind() const
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	if (ID != 0)
	{
		glDeleteVertexArrays(1, &ID);
	}
}

EBO::EBO()
{
	Count = 0;
	glGenBuffers(1, &ID);
}

EBO::EBO(const GLuint* indices, GLuint count)
{
	Count = count;
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

EBO::~EBO()
{
	Delete();
}

void EBO::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
	if (ID != 0)
	{
		glDeleteBuffers(1, &ID);
	}
}

void Renderer::Clear()
{
	glClearColor(.07f, .13f, .17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VAO& vao, GLenum mode, int vertexCount)
{
	vao.Bind();
	glDrawArrays(mode, 0, vertexCount);
}

void Renderer::Draw(const VAO& vao, const EBO& ebo)
{
	vao.Bind();
	ebo.Bind();
	glDrawElements(GL_TRIANGLES, ebo.Count, GL_UNSIGNED_INT, 0);
}

/**********************
	Shader related
***********************/

// Retrieve the Uniform location from the shader
int GetUniformLocation(const char* name)
{
	// Try to get the uniform location
	int loc = glGetUniformLocation(programID, name);

	// Check if the location exist
	if (loc == -1)
	{
		std::cout << "Warning: Cannot find uniform location of [" << name << "]" << std::endl;
		return -1;
	}

	return loc;
}

// Set 1 uniform int value
int SetUniform1i(const char* name, int value)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniform1i(loc, value);

	return 0;
}

// Set 1 uniform float value
int SetUniform1f(const char* name, float value)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniform1f(loc, value);
	return 0;
}

// Set 4 uniform float values
int SetUniform4f(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniform4f(loc, v0, v1, v2, v3);
	return 0;
}

// Set uniform mat4
int SetUniformMat4f(const char* name, const glm::mat4& matrix)
{
	int loc = GetUniformLocation(name);
	if (loc == -1)
		return -1;

	glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);
	return 0;
}

/**********************
	Generic
***********************/
class Transform
{
public:
	Transform();
	~Transform();

	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 value);

	glm::vec3 GetRotation() const;
	void SetRotation(glm::vec3 value);

	glm::vec3 GetScale() const;
	void SetScale(glm::vec3 value);
	glm::mat4 GetTransformMat4() const;

	Transform* GetParent();
	void SetParent(Transform* transform);
	std::list<Transform*> GetChilds() const;

	virtual void OnPaint();
private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	Transform* m_parent;
	std::list<Transform*> m_childs;
};

Transform::Transform() :
	m_position(glm::vec3()), m_rotation(glm::vec3()), m_scale(glm::vec3(1.f)),
	m_parent(nullptr), m_childs()
{
}

Transform::~Transform()
{
}

glm::vec3 Transform::GetPosition() const
{
	return m_position;
}

void Transform::SetPosition(glm::vec3 value)
{
	m_position = value;
}

glm::vec3 Transform::GetRotation() const
{
	return m_rotation;
}

void Transform::SetRotation(glm::vec3 value)
{
	m_rotation = value;
}

glm::vec3 Transform::GetScale() const
{
	return m_scale;
}

void Transform::SetScale(glm::vec3 value)
{
	m_scale = value;
}

void Transform::OnPaint()
{
	SetUniformMat4f("u_modelMatrix", GetTransformMat4());
}

glm::mat4 Transform::GetTransformMat4() const
{
	glm::mat4 model = glm::mat4(1.0f);

	// Translate
	model = glm::translate(model, m_position);

	// Rotate
	model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Scale
	model = glm::scale(model, m_scale);

	//static int i = 0;
	//if (i != 2)
	//{
	//	std::cout << "position: (" << m_position.x << ", " << m_position.y << ", " << m_position.z << ")" << std::endl;
	//	std::cout << "rotation: (" << m_rotation.x << ", " << m_rotation.y << ", " << m_rotation.z << ")" << std::endl;
	//	std::cout << "scale: (" << m_scale.x << ", " << m_scale.y << ", " << m_scale.z << ")" << std::endl;
	//	std::cout << "Transform: " << std::endl;
	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			std::cout << model[j][i] << " ";
	//		}
	//		std::cout << std::endl;
	//	}
	//	i++;
	//}

	return (m_parent) ? model * m_parent->GetTransformMat4() : model;
}

Transform* Transform::GetParent()
{
	return m_parent;
}

void Transform::SetParent(Transform* transform)
{
	m_parent = transform;
}

std::list<Transform*> Transform::GetChilds() const
{
	return m_childs;
}


/**********************
	Camera related
***********************/
class Camera
{
public:
	static Camera* GetMain();
	static void SetMain(Camera* camera);
	static void OnPaint();

	Camera();
	~Camera();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	Transform& GetTransform();
private:
	static Camera* m_main;

	Transform m_transform;
};

Camera* Camera::m_main = nullptr;

Camera* Camera::GetMain()
{
	return m_main;
}

void Camera::SetMain(Camera* camera)
{
	m_main = camera;
}

void Camera::OnPaint()
{
	// Set the shader's projection and view uniform
	SetUniformMat4f("u_viewMatrix", m_main->GetViewMatrix());
	SetUniformMat4f("u_projectionMatrix", m_main->GetProjectionMatrix());
}

Camera::Camera() : m_transform()
{

}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);

	// Scale (Not used)
	//view = glm::scale(view, glm::vec3(
	//	1.f / GetTransform().GetScale().x, 1.f / GetTransform().GetScale().y, 1.f / GetTransform().GetScale().z
	//));

	// Rotate
	view = glm::rotate(view, glm::radians(GetTransform().GetRotation().x * -1.f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(GetTransform().GetRotation().y * -1.f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, glm::radians(GetTransform().GetRotation().z * -1.f), glm::vec3(0.0f, 0.0f, 1.0f));

	// Translate
	view = glm::translate(view, GetTransform().GetPosition() * -1.f);

	return view;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	//return glm::ortho(0.0f, 512.0f, 0.0f, 512.0f, 0.1f, 1000.0f);
	return glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 1000.0f);
	return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
}

Transform& Camera::GetTransform()
{
	return m_transform;
}

/**********************
	Game Object related
***********************/

#define DEFAULT_COLOR_VALUE 0.8f, 0.8f, 0.8f, 1.0f

class Object
{
public:
	Object();
	~Object();

	Transform& GetTransform();

	void SetActive(bool active);
	bool IsActive() const;

	virtual void OnPaint();

private:
	Transform m_transform;
	bool m_isActive;
};

class ObjectRenderPipeline
{
public:
	static void AddObject(Object* object);
	static void RemoveObject(Object* object);
	static void OnPaint();
private:
	static std::list<Object*> m_Objects;
};

class VerticesObject : public Object
{
public:
	VerticesObject();
	VerticesObject(const GLfloat vertices[], int elementCount);
	~VerticesObject();

	virtual void SetVertices(const GLfloat vertices[], int elementCount);
	virtual void OnPaint();

protected:
	VAO* m_vao;
	VBO* m_vbo;
	int m_elementCount;
};

class ColoredVerticesObject : public VerticesObject
{
public:
	ColoredVerticesObject();
	ColoredVerticesObject(const GLfloat vertices[], int elementCount);
	~ColoredVerticesObject();

	virtual void SetVertices(const GLfloat vertices[], int elementCount);
	virtual void OnPaint();
};

class IndexedColoredVerticesObject : public ColoredVerticesObject
{
public:
	IndexedColoredVerticesObject();
	IndexedColoredVerticesObject(const GLfloat vertices[], int elementCount, const GLuint indices[], int indicesCount);
	~IndexedColoredVerticesObject();

	virtual void SetIndices(const GLuint indices[], int indicesCount);
	virtual void OnPaint();

protected:
	EBO* m_ebo;
	int m_indicesCount;
};

Object::Object() :
	m_transform(), m_isActive(false)
{
}

Object::~Object()
{
}

Transform& Object::GetTransform()
{
	return m_transform;
}

void Object::SetActive(bool active)
{
	if (active)
	{
		ObjectRenderPipeline::AddObject(this);
	}
	else
	{
		ObjectRenderPipeline::RemoveObject(this);
	}

	m_isActive = active;
}

bool Object::IsActive() const
{
	return m_isActive;
}

void Object::OnPaint()
{
	m_transform.OnPaint();
}

std::list<Object*> ObjectRenderPipeline::m_Objects;

void ObjectRenderPipeline::AddObject(Object* object)
{
	if (object->IsActive())
		return;
	m_Objects.push_back(object);
}

void ObjectRenderPipeline::RemoveObject(Object* object)
{
	if (!object->IsActive())
		return;
	m_Objects.remove(object);
}

void ObjectRenderPipeline::OnPaint()
{
	if (m_Objects.size() < 1)
		return;

	std::list<Object*>::iterator it;
	for (it = m_Objects.begin(); it != m_Objects.end(); it++)
	{
		(*it)->OnPaint();
	}
}

VerticesObject::VerticesObject() : Object(), m_vao(new VAO()), m_vbo(nullptr), m_elementCount(0)
{

}

VerticesObject::VerticesObject(const GLfloat vertices[], int elementCount) : Object(), m_vao(new VAO())
{
	SetVertices(vertices, elementCount);
}

VerticesObject::~VerticesObject()
{
	m_vao->Delete();
	m_vbo->Delete();
}

void VerticesObject::SetVertices(const GLfloat vertices[], int elementCount)
{
	m_vbo = new VBO(vertices, elementCount * sizeof(float));
	m_vao->LinkAttrib(*m_vbo, 0, 3, GL_FLOAT, 0, 0);

	m_elementCount = elementCount;
}

void VerticesObject::OnPaint()
{
	Object::OnPaint();
	SetUniform4f("u_VertColor", DEFAULT_COLOR_VALUE);
	Renderer::Draw(*m_vao, GL_TRIANGLES, m_elementCount / 3);
}

ColoredVerticesObject::ColoredVerticesObject() : VerticesObject()
{

}

ColoredVerticesObject::ColoredVerticesObject(const GLfloat vertices[], int elementCount)
{
	SetVertices(vertices, elementCount);
}

ColoredVerticesObject::~ColoredVerticesObject()
{

}

void ColoredVerticesObject::SetVertices(const GLfloat vertices[], int elementCount)
{
	m_vbo = new VBO(vertices, elementCount * sizeof(float));
	m_vao->LinkAttrib(*m_vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);
	m_vao->LinkAttrib(*m_vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	m_elementCount = elementCount;
}

void ColoredVerticesObject::OnPaint()
{
	VerticesObject::OnPaint();
}

IndexedColoredVerticesObject::IndexedColoredVerticesObject() : ColoredVerticesObject(), m_ebo(nullptr), m_indicesCount(0)
{
}

IndexedColoredVerticesObject::IndexedColoredVerticesObject(const GLfloat vertices[], int elementCount, const GLuint indices[], int indicesCount) :
	ColoredVerticesObject(vertices, elementCount)
{
	SetIndices(indices, indicesCount);
}

IndexedColoredVerticesObject::~IndexedColoredVerticesObject()
{
	m_ebo->Delete();
}

void IndexedColoredVerticesObject::SetIndices(const GLuint indices[], int indicesCount)
{
	m_ebo = new EBO(indices, indicesCount);

	m_indicesCount = indicesCount;
}

void IndexedColoredVerticesObject::OnPaint()
{
	Object::OnPaint();
	Renderer::Draw(*m_vao, *m_ebo);
}

#pragma endregion

#pragma region Assignment Specific Classes
#pragma region GameObject
class GameObject : public Object
{
public:
	virtual void OnPaint();

	template <typename T>
	static T* InstantiateOfType();
protected:
	GameObject();
	~GameObject();

	virtual GLfloat* GetVertices() const = 0;
	virtual GLuint* GetIndices() const = 0;
	virtual GLuint GetVerticesCount() const = 0;
	virtual GLuint GetIndicesCount() const = 0;
	IndexedColoredVerticesObject* m_icvo;

private:
	void Init();
};

GameObject::GameObject() : Object(), m_icvo(nullptr)
{
}

GameObject::~GameObject()
{

}

void GameObject::OnPaint()
{
	m_icvo->OnPaint();
}

template <typename T>
T* GameObject::InstantiateOfType()
{
	GameObject* go = new T();
	go->Init();
	return (T*)go;
}

void GameObject::Init()
{
	std::cout << "Sizeof Vertices: " << GetVerticesCount() * sizeof(float) << std::endl;
	std::cout << "Sizeof Indices: " << GetIndicesCount() * sizeof(int) << std::endl;
	m_icvo = new IndexedColoredVerticesObject(GetVertices(), GetVerticesCount() * sizeof(float), GetIndices(), GetIndicesCount() * sizeof(int));
	m_icvo->GetTransform().SetParent(&GetTransform());
}
#pragma endregion

#pragma region TreeStem
class Terrain : public GameObject
{
public:
	virtual GLfloat* GetVertices() const;
	virtual GLuint* GetIndices() const;
	virtual GLuint GetVerticesCount() const;
	virtual GLuint GetIndicesCount() const;

private:
	Terrain();
	~Terrain();

	friend GameObject;
};

Terrain::Terrain() : GameObject()
{

}

Terrain::~Terrain()
{

}

GLfloat* Terrain::GetVertices() const
{
	static GLfloat l_vertices[] =
	{
		.0f, 1.f, .0f,		// 0: Top Center
		.5f, .44f, .32f,

		1.f, 1.f, .0f,		// 1: 0
		.4f, .2f, .0f,

		.866f, 1.f, .5f,	// 2: 30
		.4f, .2f, .0f,

		.5f, 1.f, .866f,	// 3: 60
		.4f, .2f, .0f,

		.0f, 1.f, 1.f,		// 4: 90
		.4f, .2f, .0f,

		-.5f, 1.f, .866f,	// 5: 120
		.4f, .2f, .0f,

		-.866f, 1.f, .5f,	// 6: 150
		.4f, .2f, .0f,

		-1.f, 1.f, .0f,		// 7: 180
		.4f, .2f, .0f,

		-.866f, 1.f, -.5f,	// 8: 210
		.4f, .2f, .0f,

		-.5f, 1.f, -.866f,	// 9: 240
		.4f, .2f, .0f,

		.0f, 1.f, -1.f,		// 10: 270
		.4f, .2f, .0f,

		.5f, 1.f, -.866f,	// 11: 300
		.4f, .2f, .0f,

		.866f, 1.f, -.5f,	// 12: 330
		.4f, .2f, .0f,


		.0f, -1.f, .0f,		// 13: Bottom Center
		.5f, .44f, .32f,

		1.f, -1.f, .0f,		// 14: 0
		.5f, .2f, .0f,

		.866f, -1.f, .5f,	// 15: 30
		.4f, .26f, .08f,

		.5f, -1.f, .866f,	// 16: 60
		.5f, .2f, .0f,

		.0f, -1.f, 1.f,		// 17: 90
		.4f, .26f, .08f,

		-.5f, -1.f, .866f,	// 18: 120
		.5f, .2f, .0f,

		-.866f, -1.f, .5f,	// 19: 150
		.4f, .26f, .08f,

		-1.f, -1.f, .0f,	// 20: 180
		.5f, .2f, .0f,

		-.866f, -1.f, -.5f,	// 21: 210
		.4f, .26f, .08f,

		-.5f, -1.f, -.866f,	// 22: 240
		.5f, .2f, .0f,

		.0f, -1.f, -1.f,	// 23: 270
		.4f, .26f, .08f,

		.5f, -1.f, -.866f,	// 24: 300
		.5f, .2f, .0f,

		.866f, -1.f, -.5f,	// 25: 330
		.4f, .26f, .08f,
	};

	return l_vertices;
}

GLuint* Terrain::GetIndices() const
{
	static GLuint l_indices[] =
	{
		// Top Circle
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 9,
		0, 9, 10,
		0, 10, 11,
		0, 11, 12,
		0, 12, 1,

		// Bottom Circle
		13, 14, 15,
		13, 15, 16,
		13, 16, 17,
		13, 17, 18,
		13, 18, 19,
		13, 19, 20,
		13, 20, 21,
		13, 21, 22,
		13, 22, 23,
		13, 23, 24,
		13, 24, 25,
		13, 25, 14,

		// Sides
		1, 2, 14,
		14, 15, 2,

		2, 3, 15,
		15, 16, 3,

		3, 4, 16,
		16, 17, 4,

		4, 5, 17,
		17, 18, 5,

		5, 6, 18,
		18, 19, 6,

		6, 7, 19,
		19, 20, 7,

		7, 8, 20,
		20, 21, 8,

		8, 9, 21,
		21, 22, 9,

		9, 10, 22,
		22, 23, 10,

		10, 11, 23,
		23, 24, 11,

		11, 12, 24,
		24, 25, 12,

		12, 0, 25,
		25, 14, 0,
	};

	return l_indices;
}

GLuint Terrain::GetVerticesCount() const
{
	return 156;
}

GLuint Terrain::GetIndicesCount() const
{
	return 144;
}
#pragma endregion

#pragma region TreeStem
class TreeStem : public GameObject
{
public:
	virtual GLfloat* GetVertices() const;
	virtual GLuint* GetIndices() const;
	virtual GLuint GetVerticesCount() const;
	virtual GLuint GetIndicesCount() const;

private:
	TreeStem();
	~TreeStem();

	friend GameObject;
};

TreeStem::TreeStem() : GameObject()
{

}

TreeStem::~TreeStem()
{

}

GLfloat* TreeStem::GetVertices() const
{
	static GLfloat l_vertices[] =
	{
		.0f, 1.f, .0f,		// 0: Top Center
		.5f, .44f, .32f,

		1.f, 1.f, .0f,		// 1: 0
		.4f, .2f, .0f,

		.866f, 1.f, .5f,	// 2: 30
		.4f, .2f, .0f,

		.5f, 1.f, .866f,	// 3: 60
		.4f, .2f, .0f,

		.0f, 1.f, 1.f,		// 4: 90
		.4f, .2f, .0f,

		-.5f, 1.f, .866f,	// 5: 120
		.4f, .2f, .0f,

		-.866f, 1.f, .5f,	// 6: 150
		.4f, .2f, .0f,

		-1.f, 1.f, .0f,		// 7: 180
		.4f, .2f, .0f,

		-.866f, 1.f, -.5f,	// 8: 210
		.4f, .2f, .0f,

		-.5f, 1.f, -.866f,	// 9: 240
		.4f, .2f, .0f,

		.0f, 1.f, -1.f,		// 10: 270
		.4f, .2f, .0f,

		.5f, 1.f, -.866f,	// 11: 300
		.4f, .2f, .0f,

		.866f, 1.f, -.5f,	// 12: 330
		.4f, .2f, .0f,


		.0f, -1.f, .0f,		// 13: Bottom Center
		.5f, .44f, .32f,

		1.f, -1.f, .0f,		// 14: 0
		.5f, .2f, .0f,

		.866f, -1.f, .5f,	// 15: 30
		.4f, .26f, .08f,

		.5f, -1.f, .866f,	// 16: 60
		.5f, .2f, .0f,

		.0f, -1.f, 1.f,		// 17: 90
		.4f, .26f, .08f,

		-.5f, -1.f, .866f,	// 18: 120
		.5f, .2f, .0f,

		-.866f, -1.f, .5f,	// 19: 150
		.4f, .26f, .08f,

		-1.f, -1.f, .0f,	// 20: 180
		.5f, .2f, .0f,

		-.866f, -1.f, -.5f,	// 21: 210
		.4f, .26f, .08f,

		-.5f, -1.f, -.866f,	// 22: 240
		.5f, .2f, .0f,

		.0f, -1.f, -1.f,	// 23: 270
		.4f, .26f, .08f,

		.5f, -1.f, -.866f,	// 24: 300
		.5f, .2f, .0f,

		.866f, -1.f, -.5f,	// 25: 330
		.4f, .26f, .08f,
	};

	return l_vertices;
}

GLuint* TreeStem::GetIndices() const
{
	static GLuint l_indices[] =
	{
		// Top Circle
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 9,
		0, 9, 10,
		0, 10, 11,
		0, 11, 12,
		0, 12, 1,

		// Bottom Circle
		13, 14, 15,
		13, 15, 16,
		13, 16, 17,
		13, 17, 18,
		13, 18, 19,
		13, 19, 20,
		13, 20, 21,
		13, 21, 22,
		13, 22, 23,
		13, 23, 24,
		13, 24, 25,
		13, 25, 14,

		// Sides
		1, 2, 14,
		14, 15, 2,

		2, 3, 15,
		15, 16, 3,

		3, 4, 16,
		16, 17, 4,

		4, 5, 17,
		17, 18, 5,

		5, 6, 18,
		18, 19, 6,

		6, 7, 19,
		19, 20, 7,

		7, 8, 20,
		20, 21, 8,

		8, 9, 21,
		21, 22, 9,

		9, 10, 22,
		22, 23, 10,

		10, 11, 23,
		23, 24, 11,

		11, 12, 24,
		24, 25, 12,

		12, 0, 25,
		25, 14, 0,
	};

	return l_indices;
}

GLuint TreeStem::GetVerticesCount() const
{
	return 156;
}

GLuint TreeStem::GetIndicesCount() const
{
	return 144;
}
#pragma endregion

#pragma region Leaf
class Leaf : public GameObject
{
public:
	virtual GLfloat* GetVertices() const;
	virtual GLuint* GetIndices() const;
	virtual GLuint GetVerticesCount() const;
	virtual GLuint GetIndicesCount() const;

private:
	Leaf();
	~Leaf();

	friend GameObject;
};

Leaf::Leaf() : GameObject()
{

}

Leaf::~Leaf()
{

}

GLfloat* Leaf::GetVertices() const
{
	static GLfloat l_vertices[] =
	{
		.0f, 1.f, .0f,
		.0f, .25f, .0f,

		-1.f, -1.f, 1.f,
		.0f, .1f, .0f,

		1.f, -1.f, 1.f,
		.0f, .12f, .0f,

		1.f, -1.f, -1.f,
		.0f, .15f, .0f,

		-1.f, -1.f, -1.f,
		.0f, .13f, .0f,
	};

	return l_vertices;
}

GLuint* Leaf::GetIndices() const
{
	static GLuint l_indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
		1, 2, 3,
		3, 4, 1
	};

	return l_indices;
}

GLuint Leaf::GetVerticesCount() const
{
	return 30;
}
GLuint Leaf::GetIndicesCount() const
{
	return 18;
}
#pragma endregion


#pragma endregion


#pragma region Main OpenGL Functions
void get_OpenGL_info() {
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint programID) {
	return checkStatus(programID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID) {
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName) {
	std::ifstream meInput(fileName);
	if (!meInput.good()) {
		std::cout << "File failed to load ... " << fileName << std::endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	std::string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}


Camera* mainCamera;
TreeStem* treeStem;
Leaf* leaf;

void sendDataToOpenGL() {
	// TODO:
	// create 3D objects and/or 2D objects and/or lines (points) here and bind to VAOs & VBOs
	mainCamera = new Camera();
	mainCamera->GetTransform().SetPosition(glm::vec3(.0f, .0f, 10.f));
	Camera::SetMain(mainCamera);

	GLfloat vertCube[] =
	{
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
	};

	treeStem = GameObject::InstantiateOfType<TreeStem>();
	treeStem->SetActive(true);

	leaf = GameObject::InstantiateOfType<Leaf>();
	leaf->SetActive(true);
}

float yRotation = 0;

void paintGL(void) {
	// always run
	// TODO:
	// render your objects and control the transformation here

	leaf->GetTransform().SetRotation(glm::vec3(.0f, yRotation, .0f));

	yRotation += 1;

	Renderer::Clear();
	Camera::OnPaint();
	ObjectRenderPipeline::OnPaint();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO:
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Camera Moving
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		mainCamera->GetTransform().SetPosition(mainCamera->GetTransform().GetPosition() + glm::vec3(.0f, 1.f, .0f));
	}
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		mainCamera->GetTransform().SetPosition(mainCamera->GetTransform().GetPosition() + glm::vec3(-1.0f, 0.f, .0f));
	}

	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		mainCamera->GetTransform().SetPosition(mainCamera->GetTransform().GetPosition() + glm::vec3(.0f, -1.f, .0f));
	}
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		mainCamera->GetTransform().SetPosition(mainCamera->GetTransform().GetPosition() + glm::vec3(1.0f, 0.f, .0f));
	}

	// Camera turnning
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		mainCamera->GetTransform().SetRotation(mainCamera->GetTransform().GetRotation() + glm::vec3(10.f, .0f, .0f));
	}
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		mainCamera->GetTransform().SetRotation(mainCamera->GetTransform().GetRotation() + glm::vec3(.0f, 10.f, .0f));
	}

	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		mainCamera->GetTransform().SetRotation(mainCamera->GetTransform().GetRotation() + glm::vec3(-10.f, .0f, .0f));
	}
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		mainCamera->GetTransform().SetRotation(mainCamera->GetTransform().GetRotation() + glm::vec3(.0f, -10.f, .0f));
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void initializedGL(void) {
	// run only once
	// TODO:
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char* argv[]) {
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* do not allow resizing */
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "Assignment 1", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback); // TODO
	// TODO: mouse callback, etc.

	/* Initialize the glew */
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	get_OpenGL_info();
	initializedGL();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
#pragma endregion
