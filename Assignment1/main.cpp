/*
Type your name and student ID here
	- Name: SZE-TO Kwok Leung
	- Student ID: 1155149068
*/

// Enjoy this 2000+ lines of code

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <list>
#include <unordered_map>

GLint programID;

#pragma region My API
/***************************************************************
	Classes and functions created by me to simplify some codes
****************************************************************/

#pragma region Render Related
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

#define DEFAULT_CLEAR_COLOR 

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
	// Set the clear color
	static void SetClearColor(glm::vec4 clearColor);
private:
	static glm::vec4 s_clearColor;
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

glm::vec4 Renderer::s_clearColor = glm::vec4(DEFAULT_CLEAR_COLOR);

void Renderer::Clear()
{
	glClearColor(s_clearColor.x, s_clearColor.y, s_clearColor.z, s_clearColor.w);
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
void Renderer::SetClearColor(glm::vec4 clearColor)
{
	s_clearColor = clearColor;
}
#pragma endregion

#pragma region Shader Related
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
#pragma endregion

#pragma region Generic
/**********************
	Generic
***********************/
class Transform
{
public:
	Transform();
	~Transform();

	// World Space. Parent's transform will be taken into account
	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 value);

	glm::vec3 GetRotation() const;
	void SetRotation(glm::vec3 value);

	glm::vec3 GetScale() const;
	void SetScale(glm::vec3 value);

	// Local space
	glm::vec3 GetLocalPosition() const;
	void SetLocalPosition(glm::vec3 value);

	glm::vec3 GetLocalRotation() const;
	void SetLocalRotation(glm::vec3 value);

	glm::vec3 GetLocalScale() const;
	void SetLocalScale(glm::vec3 value);

	glm::mat4 GetTransformMat4() const;

	glm::vec3 GetForward() const;
	glm::vec3 GetBackward() const;
	glm::vec3 GetLeft() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetDown() const;

	Transform* GetParent();
	void SetParent(Transform* transform);
	std::list<Transform*> GetChilds() const;

	virtual void OnPaint();
private:
	glm::vec3 m_localPosition;
	glm::vec3 m_localRotation;
	glm::vec3 m_localScale;

	Transform* m_parent;
	std::list<Transform*> m_childs;
};

Transform::Transform() :
	m_localPosition(glm::vec3()), m_localRotation(glm::vec3()), m_localScale(glm::vec3(1.f)),
	m_parent(nullptr), m_childs()
{
}

Transform::~Transform()
{
}

glm::vec3 Transform::GetPosition() const
{
	if (m_parent != nullptr)
	{
		return m_parent->GetPosition() + m_localPosition;
	}

	return m_localPosition;
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
		return glm::vec3(m_parent->GetScale().x * m_localScale.x, m_parent->GetScale().y * m_localScale.y, m_parent->GetScale().z * m_localScale.z);
	}

	return m_localScale;
}

void Transform::SetScale(glm::vec3 value)
{
	if (m_parent != nullptr)
	{
		m_localScale = glm::vec3(value.x / m_parent->GetScale().x, value.y / m_parent->GetScale().y, value.z / m_parent->GetScale().z);
	}
	else
	{
		m_localScale = value;
	}
}

void Transform::OnPaint()
{
	SetUniformMat4f("u_modelMatrix", GetTransformMat4());
}

glm::mat4 Transform::GetTransformMat4() const
{
	glm::mat4 model = glm::mat4(1.0f);

	// Scale
	model = glm::scale(model, GetLocalScale());

	// Rotate
	model = glm::rotate(model, glm::radians(GetLocalRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(GetLocalRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(GetLocalRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Translate
	model = glm::translate(model, GetLocalPosition());

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

	if (m_parent != nullptr)
	{
		return m_parent->GetTransformMat4() * model;
	}

	return model;
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
	return glm::vec3(
		glm::cos(glm::radians(GetRotation().x)) * glm::sin(glm::radians(GetRotation().y)),
		-1.f * glm::sin(glm::radians(GetRotation().x)),
		glm::cos(glm::radians(GetRotation().x)) * glm::cos(glm::radians(GetRotation().y))
	);
}

glm::vec3 Transform::GetRight() const
{
	return glm::vec3(
		glm::cos(glm::radians(GetRotation().y)),
		0,
		-1.f * glm::sin(glm::radians(GetRotation().y))
	);
}

glm::vec3 Transform::GetLeft() const
{
	return -1.f * GetRight();
}

glm::vec3 Transform::GetUp() const
{
	return glm::cross(GetForward(), GetRight());
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
	transform->m_childs.push_back(this);
}

std::list<Transform*> Transform::GetChilds() const
{
	return m_childs;
}
#pragma endregion

#pragma region Camera Related
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
	static Camera* s_main;

	Transform m_transform;
};

Camera* Camera::s_main = nullptr;

Camera* Camera::GetMain()
{
	return s_main;
}

void Camera::SetMain(Camera* camera)
{
	s_main = camera;
}

void Camera::OnPaint()
{
	// Set the shader's projection and view uniform
	SetUniformMat4f("u_viewMatrix", s_main->GetViewMatrix());
	SetUniformMat4f("u_projectionMatrix", s_main->GetProjectionMatrix());
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
#pragma endregion

#pragma region GameObject Related
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

	virtual void SetActive(bool active);
	virtual bool IsActive() const;

	virtual void OnPaint();

protected:
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
	static std::list<Object*> s_objects;
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

std::list<Object*> ObjectRenderPipeline::s_objects;

void ObjectRenderPipeline::AddObject(Object* object)
{
	if (object->IsActive())
		return;
	s_objects.push_back(object);
}

void ObjectRenderPipeline::RemoveObject(Object* object)
{
	if (!object->IsActive())
		return;
	s_objects.remove(object);
}

void ObjectRenderPipeline::OnPaint()
{
	if (s_objects.size() < 1)
		return;

	std::list<Object*>::iterator it;
	for (it = s_objects.begin(); it != s_objects.end(); it++)
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

	delete m_vao;
	delete m_vbo;
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

	delete m_ebo;
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

class GameObject : public Object
{
public:
	virtual void OnPaint();

	template <typename T>
	static T* InstantiateOfType();
	static void Destroy(GameObject* gameObject);
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
	delete m_icvo;
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

void GameObject::Destroy(GameObject* gameObject)
{
	delete gameObject;
}

void GameObject::Init()
{
	m_icvo = new IndexedColoredVerticesObject(GetVertices(), GetVerticesCount() * sizeof(float), GetIndices(), GetIndicesCount() * sizeof(int));
	m_icvo->GetTransform().SetParent(&GetTransform());
}
#pragma endregion

#pragma region Scene
class Scene
{
public:
	Scene();
	~Scene();

	virtual void OnInitialize();
	virtual void OnPaint();
	virtual void OnEnd();

	void AddObject(Object* object);
	void RemoveObject(Object* object);
	std::list<Object*> GetObjects() const;

protected:
	std::list<Object*> m_objects;
	bool m_initialized;
};

class SceneManager
{
public:
	static void OnInitialize();
	static void OnPaint();
	static void OnEnd();
	static void SetActiveScene(Scene* scene);
	static Scene* GetActiveScene();

private:
	static Scene* s_activeScene;
	static bool s_busying;
};

Scene* SceneManager::s_activeScene = nullptr;
bool SceneManager::s_busying = false;

void SceneManager::OnInitialize()
{
	if (s_activeScene != nullptr)
	{
		s_activeScene->OnInitialize();
	}
}

void SceneManager::OnPaint()
{
	if (s_busying)
	{
		return;
	}

	if (s_activeScene != nullptr)
	{
		s_activeScene->OnPaint();
	}
}

void SceneManager::OnEnd()
{
	// End the current scene
	if (s_activeScene != nullptr)
	{
		s_activeScene->OnEnd();
	}
}

void SceneManager::SetActiveScene(Scene* scene)
{
	s_busying = true;

	// End the current scene
	if (s_activeScene != nullptr)
	{
		s_activeScene->OnEnd();
	}

	// Set the current scene
	s_activeScene = scene;

	// Initialize the current scene
	if (s_activeScene != nullptr)
	{
		s_activeScene->OnInitialize();
	}

	s_busying = false;
}

Scene* SceneManager::GetActiveScene()
{
	return s_activeScene;
}

Scene::Scene() : m_objects(), m_initialized(false)
{
}

Scene::~Scene()
{
	while (m_objects.size() > 0)
	{
		Object* o = *(m_objects.begin());
		m_objects.remove(o);
		delete o;
	}
}

void Scene::OnInitialize()
{
	if (m_initialized)
		return;

	m_initialized = true;
}

void Scene::OnPaint()
{
}

void Scene::OnEnd()
{
}

void Scene::AddObject(Object* object)
{
	m_objects.push_back(object);
}

void Scene::RemoveObject(Object* object)
{
	m_objects.remove(object);
}

std::list<Object*> Scene::GetObjects() const
{
	return m_objects;
}
#pragma endregion

#pragma region Physics Related
class Physics
{
public:
	bool Raycast(glm::vec3 origin, glm::vec3 direction, GLfloat maxDistance);
};
#pragma endregion

#pragma region Input Related
typedef void(*KeyCallbackFunc)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void(*CursorPosCallbackFunc)(GLFWwindow* window, double xpos, double ypos);
typedef void(*MouseButtonCallbackFunc)(GLFWwindow* window, int button, int action, double xpos, int ypos);

// A event listener that listen to the input event
class KeyCallback
{
public:
	KeyCallback(void);
	KeyCallback(KeyCallbackFunc func);
	void SetCallback(KeyCallbackFunc func);
private:
	// Allow the Input Manager to access the callback function
	friend class Input;

	// The key to this callback listener in the input manager
	int m_id;
	KeyCallbackFunc m_callback;
};

class CursorPosCallback
{
public:
	CursorPosCallback(void);
	CursorPosCallback(CursorPosCallbackFunc func);
	void SetCallback(CursorPosCallbackFunc func);

private:
	friend class Input;

	int m_id;
	CursorPosCallbackFunc m_callback;
};

class MouseButtonCallback
{
public:
	MouseButtonCallback(void);
	MouseButtonCallback(MouseButtonCallbackFunc func);
	void SetCallback(MouseButtonCallbackFunc func);

private:
	friend class Input;

	int m_id;
	MouseButtonCallbackFunc m_callback;
};

// The Input Manager to handle all the input events
class Input
{
public:
	static void Init(GLFWwindow* window);

	// Keyboard keys callback
	static void AddKeyCallback(KeyCallback callback);
	static void RemoveKeyCallback(KeyCallback callback);
	static void RemoveAllKeyCallbacks();

	// Mouse button callback
	static void AddMouseButtonCallback(MouseButtonCallback callback);
	static void RemoveMouseButtonCallback(MouseButtonCallback callback);
	static void RemoveAllMouseButtonCallbacks();

	// Mouse move callback
	static void AddCursorPosCallback(CursorPosCallback callback);
	static void RemoveCursorPosCallback(CursorPosCallback callback);
	static void RemoveAllCursorPosCallbacks();

private:
	static unsigned int s_nextKey;
	static std::unordered_map<int, KeyCallback> s_keyCallbacks;
	static std::unordered_map<int, CursorPosCallback> s_cursorPosCallbacks;
	static std::unordered_map<int, MouseButtonCallback> s_mouseButtonCallbacks;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

KeyCallback::KeyCallback(void) : m_id(-1), m_callback(nullptr)
{
}

// Constructor of the Key Callback Listener
KeyCallback::KeyCallback(KeyCallbackFunc func) : m_id(-1)
{
	SetCallback(func);
}

void KeyCallback::SetCallback(KeyCallbackFunc func)
{
	m_callback = func;
}

CursorPosCallback::CursorPosCallback(void) : m_id(-1), m_callback(nullptr)
{
}

CursorPosCallback::CursorPosCallback(CursorPosCallbackFunc func) : m_id(-1)
{
	SetCallback(func);
}

void CursorPosCallback::SetCallback(CursorPosCallbackFunc func)
{
	m_callback = func;
}

MouseButtonCallback::MouseButtonCallback(void) : m_id(-1), m_callback(nullptr)
{
}

MouseButtonCallback::MouseButtonCallback(MouseButtonCallbackFunc func) : m_id(-1)
{
	SetCallback(func);
}

void MouseButtonCallback::SetCallback(MouseButtonCallbackFunc func)
{
	m_callback = func;
}

unsigned int Input::s_nextKey = 0;
std::unordered_map<int, KeyCallback> Input::s_keyCallbacks;
std::unordered_map<int, CursorPosCallback> Input::s_cursorPosCallbacks;
std::unordered_map<int, MouseButtonCallback> Input::s_mouseButtonCallbacks;

// The key callback function which is binded to glfw
void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Invoke all the callback listeners
	for (unsigned int i = 0; i < s_keyCallbacks.size(); i++)
	{
		if (s_keyCallbacks[i].m_callback)
			s_keyCallbacks[i].m_callback(window, key, scancode, action, mods);
	}
}

void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Invoke all the callback listeners
	for (unsigned int i = 0; i < s_cursorPosCallbacks.size(); i++)
	{
		if (s_cursorPosCallbacks[i].m_callback)
			s_cursorPosCallbacks[i].m_callback(window, xpos, ypos);
	}
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Get the cursor position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Invoke all the callback listeners
	for (unsigned int i = 0; i < s_mouseButtonCallbacks.size(); i++)
	{
		if (s_mouseButtonCallbacks[i].m_callback)
			s_mouseButtonCallbacks[i].m_callback(window, button, action, xpos, ypos);
	}
}

// Initialize the Input Manager
void Input::Init(GLFWwindow* window)
{
	s_nextKey = 0;
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void Input::AddKeyCallback(KeyCallback callback)
{
	if (callback.m_id != -1)
		return;

	KeyCallback newCallback(callback);
	s_keyCallbacks[s_nextKey] = newCallback;
	callback.m_id = s_nextKey;
	s_nextKey++;
}

void Input::RemoveKeyCallback(KeyCallback callback)
{
	if (callback.m_id == -1)
		return;

	s_keyCallbacks.erase(callback.m_id);
	callback.m_id = -1;
}

void Input::RemoveAllKeyCallbacks()
{
	while (s_keyCallbacks.size() > 0)
	{
		RemoveKeyCallback(s_keyCallbacks.at(0));
	}
}
void Input::AddMouseButtonCallback(MouseButtonCallback callback)
{
	if (callback.m_id != -1)
		return;

	MouseButtonCallback newCallback(callback);
	s_mouseButtonCallbacks[s_nextKey] = newCallback;
	callback.m_id = s_nextKey;
	s_nextKey++;
}

void Input::RemoveMouseButtonCallback(MouseButtonCallback callback)
{
	if (callback.m_id == -1)
		return;

	s_mouseButtonCallbacks.erase(callback.m_id);
	callback.m_id = -1;
}

void Input::RemoveAllMouseButtonCallbacks()
{
	while (s_mouseButtonCallbacks.size() > 0)
	{
		RemoveMouseButtonCallback(s_mouseButtonCallbacks.at(0));
	}
}

void Input::AddCursorPosCallback(CursorPosCallback callback)
{
	if (callback.m_id != -1)
		return;

	CursorPosCallback newCallback(callback);
	s_cursorPosCallbacks[s_nextKey] = newCallback;
	callback.m_id = s_nextKey;
	s_nextKey++;
}

void Input::RemoveCursorPosCallback(CursorPosCallback callback)
{
	if (callback.m_id == -1)
		return;

	s_cursorPosCallbacks.erase(callback.m_id);
	callback.m_id = -1;
}

void Input::RemoveAllCursorPosCallbacks()
{
	while (s_cursorPosCallbacks.size() > 0)
	{
		RemoveCursorPosCallback(s_cursorPosCallbacks.at(0));
	}
}

#pragma endregion

#pragma endregion

#pragma region Assignment Specific Classes
#pragma region Terrain
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
		// Top
		-1.f, 1.f, 1.f,
		.0f, .6f, .0f,

		1.f, 1.f, 1.f,
		.0f, .55f, .0f,

		1.f, 1.f, -1.f,
		.0f, .3f, .0f,

		-1.f, 1.f, -1.f,
		.0f, .4f, .0f,

		// Bottom
		-1.f, -1.f, 1.f,
		.3f, .2f, .0f,

		1.f, -1.f, 1.f,
		.3f, .2f, .0f,

		1.f, -1.f, -1.f,
		.3f, .2f, .0f,

		-1.f, -1.f, -1.f,
		.3f, .2f, .0f,
	};

	return l_vertices;
}

GLuint Terrain::GetVerticesCount() const
{
	return 48;
}

GLuint* Terrain::GetIndices() const
{
	static GLuint l_indices[] =
	{
		// Top
		0, 1, 2,
		2, 3, 0,

		// Bottom
		4, 5, 6,
		6, 7, 4,

		// Front
		0, 1, 4,
		4, 5, 1,

		// Right
		1, 2, 5,
		5, 6, 2,

		// Back
		2, 3, 6,
		6, 7, 3,

		// Left
		3, 0, 7,
		7, 4, 0,
	};

	return l_indices;
}

GLuint Terrain::GetIndicesCount() const
{
	return 36;
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

		12, 1, 25,
		25, 14, 1,
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

#pragma region Tree
#define TREE_MAX_GENERATION 4
class Tree : public Object
{
public:
	Tree();
	~Tree();

	virtual void SetActive(bool active);
	virtual bool IsActive() const;

	void Grow();
	void Chop();
	void SetGeneration(int gen);
	int GetGeneration() const;

private:
	std::list<Leaf*> m_leaves;
	TreeStem* m_stem;

	int m_generation;
	bool m_chopped;
};

Tree::Tree() : Object(), m_leaves(), m_stem(GameObject::InstantiateOfType<TreeStem>()), m_generation(1), m_chopped(false)
{
	// Set stem transform and active
	m_stem->GetTransform().SetLocalPosition(glm::vec3(.0f, -.5f, .0f));
	m_stem->GetTransform().SetLocalScale(glm::vec3(.3f, 1.f, .3f));
	m_stem->GetTransform().SetParent(&GetTransform());
	m_stem->SetActive(true);

	// Set leaf position
	for (float i = 1.0f; i < TREE_MAX_GENERATION + 1; i++)
	{
		Leaf* leaf = GameObject::InstantiateOfType<Leaf>();
		leaf->GetTransform().SetParent(&GetTransform());
		m_leaves.push_back(leaf);
	}

	// Set the tree's generation
	SetGeneration(m_generation);
}

Tree::~Tree()
{

}

void Tree::SetActive(bool active)
{
	m_stem->SetActive(active);

	std::list<Leaf*>::iterator it = m_leaves.begin();
	for (int i = 0; i < m_generation; i++, it++)
	{
		(*it)->SetActive(active);
	}
	m_isActive = active;
}

bool Tree::IsActive() const
{
	return m_isActive;
}

void Tree::Grow()
{
	if (m_chopped)
		return;

	if (m_generation < TREE_MAX_GENERATION)
	{
		SetGeneration(++m_generation);
	}
}

void Tree::Chop()
{
	m_chopped = true;

	std::list<Leaf*>::iterator it = m_leaves.begin();
	for (int i = 0; i < m_generation; i++, it++)
	{
		(*it)->SetActive(false);
	}
}

void Tree::SetGeneration(int gen)
{
	// Set the scale of the stem
	m_stem->GetTransform().SetLocalScale(glm::vec3(.3f * gen * .7f, gen / 5.f, .3f * gen * .7f));

	std::list<Leaf*>::iterator it = m_leaves.begin();
	for (int i = 0; i < m_generation; i++, it++)
	{
		// Set the scale of the leaves
		(*it)->GetTransform().SetLocalScale(glm::vec3(.5f * gen * .8f, .8f * gen * .6f, .5f * gen * .8f));

		// Set the position of the leaves
		(*it)->GetTransform().SetLocalPosition(glm::vec3(.0f, i + i * .1f + 1, .0f));

		// Set leaves active
		(*it)->SetActive(i < gen);
	}

	m_generation = gen;
}

int Tree::GetGeneration() const
{
	return m_generation;
}
#pragma endregion

#pragma region Cloud
class Cloud : public GameObject
{
public:
	virtual GLfloat* GetVertices() const;
	virtual GLuint* GetIndices() const;
	virtual GLuint GetVerticesCount() const;
	virtual GLuint GetIndicesCount() const;

private:
	Cloud();
	~Cloud();

	friend GameObject;
};

Cloud::Cloud() : GameObject()
{

}

Cloud::~Cloud()
{

}

GLfloat* Cloud::GetVertices() const
{
	static GLfloat l_vertices[] =
	{
		// Top
		-1.f, 1.f, 1.f,
		1.0f, 1.f, 1.f,

		1.f, 1.f, 1.f,
		.8f, .8f, .8f,

		1.f, 1.f, -1.f,
		.9f, .9f, .9f,

		.0f, 1.f, -1.f,
		.9f, .9f, .9f,

		.0f, 1.f, -.5f,
		.85f, .85f, .85f,

		-1.f, 1.f, -.5f,
		.85f, .85f, .85f,

		// Bottom
		-1.f, .0f, 1.f,
		1.0f, 1.f, 1.f,

		1.f, .0f, 1.f,
		1.0f, 1.f, 1.f,

		1.f, .0f, -1.f,
		.9f, .9f, .9f,

		.0f, .0f, -1.f,
		.9f, .9f, .9f,

		.0f, .0f, -.5f,
		.85f, .85f, .85f,

		-1.f, .0f, -.5f,
		.85f, .85f, .85f,
	};

	return l_vertices;
}

GLuint* Cloud::GetIndices() const
{
	static GLuint l_indices[] =
	{
		// Top
		0, 1, 5,
		1, 5, 4,
		1, 4, 3,
		1, 3, 2,

		// Botoom
		6, 7, 11,
		7, 11, 10,
		7, 10, 9,
		7, 9, 8,

		// Left
		0, 5, 6,
		6, 5, 11,
		3, 4, 10,
		9, 10, 3,

		// Back
		2, 3, 8,
		2, 9, 8,
		4, 5, 11,
		4, 11, 10,

		// Front
		0, 7, 6,
		0, 1, 7,

		// Right
		1, 2, 8,
		1, 8, 7,
	};

	return l_indices;
}

GLuint Cloud::GetVerticesCount() const
{
	return 72;
}
GLuint Cloud::GetIndicesCount() const
{
	return 60;
}
#pragma endregion

#pragma region AxeBlade
class AxeBlade : public GameObject
{
public:
	virtual GLfloat* GetVertices() const;
	virtual GLuint* GetIndices() const;
	virtual GLuint GetVerticesCount() const;
	virtual GLuint GetIndicesCount() const;

private:
	AxeBlade();
	~AxeBlade();

	friend GameObject;
};

AxeBlade::AxeBlade() : GameObject()
{

}

AxeBlade::~AxeBlade()
{

}

GLfloat* AxeBlade::GetVertices() const
{
	static GLfloat l_vertices[] =
	{
		.0f, -.5f, -.8f,
		-.1f, -.2f, .0f,

	};

	return l_vertices;
}

GLuint* AxeBlade::GetIndices() const
{
	static GLuint l_indices[] =
	{
		0, 1, 3,
	};

	return l_indices;
}

GLuint AxeBlade::GetVerticesCount() const
{
	return 72;
}
GLuint AxeBlade::GetIndicesCount() const
{
	return 60;
}
#pragma endregion

#pragma region Player Controller
class PlayerController : public Object
{
public:
	PlayerController();
	~PlayerController();

	void SetVelocity(glm::vec3 value);
	glm::vec3 GetVelocity() const;

	virtual void SetActive(bool active);
	virtual bool IsActive() const;

	virtual void OnPaint();

	void SetCamera(Camera* camera);
	void SetBody(Transform* body);

private:
	static PlayerController* s_activeController;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

	float m_speed;
	float m_controlSpeed;
	glm::vec3 m_velocity;
	Camera* m_camera;
	Transform* m_body;
};

PlayerController::PlayerController() : m_velocity(glm::vec3()), m_camera(nullptr), m_body(nullptr), m_speed(.1f), m_controlSpeed(.1f)
{
	static bool init = false;
	if (!init)
	{
		Input::AddKeyCallback(PlayerController::key_callback);
		Input::AddCursorPosCallback(PlayerController::cursor_pos_callback);
	}
}

PlayerController::~PlayerController()
{
}

void PlayerController::SetVelocity(glm::vec3 value)
{
	m_velocity = value;
}

glm::vec3 PlayerController::GetVelocity() const
{
	return glm::vec3();
}

void PlayerController::SetActive(bool active)
{
	Object::SetActive(active);

	if (active)
	{
		s_activeController = this;
		Camera::SetMain(m_camera);
	}
	else if (s_activeController == this)
	{
		s_activeController = nullptr;
		Camera::SetMain(nullptr);
	}
}

bool PlayerController::IsActive() const
{
	return PlayerController::s_activeController == this;
}

void PlayerController::OnPaint()
{
	m_body->SetPosition(m_body->GetPosition() + m_velocity);
}

void PlayerController::SetCamera(Camera* camera)
{
	m_camera = camera;
}

void PlayerController::SetBody(Transform* body)
{
	m_body = body;
}

PlayerController* PlayerController::s_activeController = nullptr;

void PlayerController::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (s_activeController == nullptr)
	{
		return;
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			s_activeController->m_velocity = s_activeController->m_body->GetForward();
			break;

		case GLFW_KEY_A:
			s_activeController->m_velocity = s_activeController->m_body->GetLeft();
			break;

		case GLFW_KEY_D:
			s_activeController->m_velocity = s_activeController->m_body->GetRight();
			break;

		case GLFW_KEY_S:
			s_activeController->m_velocity = s_activeController->m_body->GetBackward();
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		s_activeController->m_velocity = glm::vec3();
	}
}

void PlayerController::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (s_activeController == nullptr)
	{
		return;
	}

	static int wWidth, wHeight; // Window width and height

	// Get the center position of the window
	glfwGetWindowSize(window, &wWidth, &wHeight);
	static glm::vec2 center = glm::vec2(wWidth / 2, wHeight / 2);

	// Do not proceed if the position is the center of the window
	if (xpos == center.x && ypos == center.y)
		return;

	PlayerController* pc = PlayerController::s_activeController;

	// Get Main Camera
	Camera* cam = pc->m_camera;
	// Get Controller Body
	Transform* body = pc->m_body;

	// handle the mouse input
	glm::vec2 newPos = glm::vec2(xpos, ypos);
	glm::vec2 deltaPos = newPos - center;

	glm::vec3 cameraRotation = cam->GetTransform().GetRotation();
	cameraRotation += glm::vec3(deltaPos.y * pc->m_controlSpeed * -1.f, .0f, .0f);

	glm::vec3 bodyRotation = body->GetRotation();
	bodyRotation += glm::vec3(.0f, deltaPos.x * pc->m_controlSpeed * -1.f, .0f);

	// Clamp the rotation value
	if (cameraRotation.x >= 360.f || cameraRotation.x <= -360.f)
		cameraRotation.x = .0f;
	if (bodyRotation.y >= 360.f || bodyRotation.y <= -360.f)
		bodyRotation.y = .0f;

	// Set the rotation of the camera
	cam->GetTransform().SetRotation(cameraRotation);
	body->SetRotation(bodyRotation);

	// Fix the cursor position to the center of the screen
	glfwSetCursorPos(window, wWidth / 2, wHeight / 2);
}

#pragma endregion

#pragma region First Person Player
class FirstPersonPlayer : public Object
{
public:
	FirstPersonPlayer();
	~FirstPersonPlayer();

	virtual void SetActive(bool active);
	virtual bool IsActive() const;

private:
	Camera* m_camera;
	PlayerController* m_controller;
};

FirstPersonPlayer::FirstPersonPlayer() : Object(), m_camera(new Camera()), m_controller(new PlayerController())
{
	m_camera->GetTransform().SetPosition(glm::vec3(.0f, .5f, .0f));
	m_camera->GetTransform().SetParent(&GetTransform());

	m_controller->SetBody(&GetTransform());
	m_controller->SetCamera(m_camera);
}

FirstPersonPlayer::~FirstPersonPlayer()
{
	delete m_camera;
	delete m_controller;
}

void FirstPersonPlayer::SetActive(bool active)
{
	Object::SetActive(active);
	m_controller->SetActive(active);
	if (active)
	{
		Camera::SetMain(m_camera);
	}
	else 
	{
		if (Camera::GetMain() == m_camera)
		{
			Camera::SetMain(nullptr);
		}
	}
}

bool FirstPersonPlayer::IsActive() const
{
	return m_isActive;
}

#pragma endregion

#pragma region Forest Scene
class ForestScene : public Scene
{
public:
	ForestScene();
	~ForestScene();

	virtual void OnInitialize();
	virtual void OnPaint();
	virtual void OnEnd();

	static ForestScene* GetInstance();

private:
	static ForestScene* s_instnace;

	FirstPersonPlayer* m_player;
	Terrain* m_terrain;
	std::list<Tree*> m_trees;
	std::list<Cloud*> m_clouds;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
};

ForestScene::ForestScene()
{
	//// Create Camera
	//m_mainCamera = new Camera();
	//m_mainCamera->GetTransform().SetPosition(glm::vec3(4.0f, 3.0f, 10.f));
	//m_mainCamera->GetTransform().SetRotation(glm::vec3(-10.f, 20.0f, .0f));
	//Camera::SetMain(m_mainCamera);

	// First Person Controller
	m_player = new FirstPersonPlayer();
	m_player->GetTransform().SetLocalPosition(glm::vec3(.0f, 1.f, .0f));
	m_player->SetActive(true);

	// Create terrain
	m_terrain = GameObject::InstantiateOfType<Terrain>();
	m_terrain->GetTransform().SetScale(glm::vec3(50.f, .5f, 50.f));
	m_terrain->GetTransform().SetPosition(glm::vec3(1.f, .0f, 1.f));
	m_terrain->SetActive(true);
	AddObject(m_terrain);

	for (int x = 5; x < 100; x += 3)
	{
		for (int y = 5; y < 100; y += 3)
		{
			int r = rand() % 100;

			if (r < 60)
			{
				// Create Trees
				Tree* newTree = new Tree();
				newTree->GetTransform().SetPosition(glm::vec3((float)x, .8f, (float)y));
				m_trees.push_back(newTree);
				newTree->SetActive(true);
				AddObject(newTree);
			}

			if (r < 90)
			{
				// Create Clouds
				int randScale = rand() % 100;
				Cloud* newCloud = GameObject::InstantiateOfType<Cloud>();
				newCloud->GetTransform().SetLocalScale(glm::vec3(randScale, randScale * .2f, randScale));
				newCloud->GetTransform().SetLocalRotation(glm::vec3(.0f, .0f, .0f));
				newCloud->GetTransform().SetLocalPosition(glm::vec3(x, 100.f, y));
				m_clouds.push_back(newCloud);
				newCloud->SetActive(true);
				AddObject(newCloud);
			}
		}
	}

	ForestScene::s_instnace = this;
}
ForestScene::~ForestScene()
{
	
}

void ForestScene::OnInitialize()
{
	// Add Input Listener
	Input::AddKeyCallback(ForestScene::key_callback);
	Input::AddCursorPosCallback(ForestScene::cursor_pos_callback);
}

void ForestScene::OnPaint() 
{
	static glm::vec3 cloudSpeed = glm::vec3(.0f, .0f, .05f);
	static float cloudBound = 1.f;

	// Move the clouds
	std::list<Cloud*>::iterator itc;
	for (itc = m_clouds.begin(); itc != m_clouds.end(); itc++)
	{
		glm::vec3 des = (*itc)->GetTransform().GetPosition() + cloudSpeed;

		// Set the bound of the cloud
		if (des.z >= 110.f)
		{
			des.z = -1.f;
		}
		(*itc)->GetTransform().SetPosition(des);
	}

	// Grow the trees by chance
	int r = rand() % 100;
	if (r > 90)
	{
		r = rand() % m_trees.size();
		std::list<Tree*>::iterator itt = m_trees.begin();
		for (int i = 0; i < r; i++, itt++);

		(*itt)->Grow();
	}
}

void ForestScene::OnEnd()
{
}

ForestScene* ForestScene::GetInstance()
{
	return s_instnace;
}

ForestScene* ForestScene::s_instnace = nullptr;

void ForestScene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void ForestScene::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	
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

ForestScene* fs;

void sendDataToOpenGL() {
	// TODO:
	// create 3D objects and/or 2D objects and/or lines (points) here and bind to VAOs & VBOs
	Renderer::SetClearColor(glm::vec4(.5f, .8f, .9f, 1.f));

	fs = new ForestScene();
	SceneManager::SetActiveScene(fs);

	// Set clear color

	SceneManager::OnInitialize();
}

void paintGL(void) {
	// always run
	// TODO:
	// render your objects and control the transformation here

	Renderer::Clear();
	Camera::OnPaint();
	ObjectRenderPipeline::OnPaint();
	SceneManager::OnPaint();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO:
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
	//glfwSetKeyCallback(window, key_callback); // TODO
	// TODO: mouse callback, etc.
	// Hide mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	Input::Init(window);
	Input::AddKeyCallback(KeyCallback(key_callback));

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
