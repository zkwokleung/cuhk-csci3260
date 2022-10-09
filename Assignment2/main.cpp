/*
Student Information
Student ID:
Student Name:
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>

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

	virtual void OnPaint(Shader* shader);
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

void Transform::OnPaint(Shader* shader)
{
	glm::mat4 m = GetTransformMat4();
	shader->setMat4("u_modelMatrix", m);
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
	static void OnPaint(Shader* shader);

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

void Camera::OnPaint(Shader* shader)
{
	// Set the shader's projection and view uniform
	glm::mat4 v = s_main->GetViewMatrix();
	glm::mat4 p = s_main->GetProjectionMatrix();
	shader->setMat4("u_viewMatrix", v);
	shader->setMat4("u_projectionMatrix", p);
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

	virtual void OnPaint(Shader* shader);

protected:
	Transform m_transform;
	bool m_isActive;
};

class ObjectRenderPipeline
{
public:
	static void AddObject(Object* object);
	static void RemoveObject(Object* object);
	static void OnPaint(Shader* shader);
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
	virtual void OnPaint(Shader* shader);

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
	virtual void OnPaint(Shader* shader);
};

class IndexedColoredVerticesObject : public ColoredVerticesObject
{
public:
	IndexedColoredVerticesObject();
	IndexedColoredVerticesObject(const GLfloat vertices[], int elementCount, const GLuint indices[], int indicesCount);
	~IndexedColoredVerticesObject();

	virtual void SetIndices(const GLuint indices[], int indicesCount);
	virtual void OnPaint(Shader* shader);

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

void Object::OnPaint(Shader* shader)
{
	m_transform.OnPaint(shader);
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

void ObjectRenderPipeline::OnPaint(Shader* shader)
{
	if (s_objects.size() < 1)
		return;

	std::list<Object*>::iterator it;
	for (it = s_objects.begin(); it != s_objects.end(); it++)
	{
		(*it)->OnPaint(shader);
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

void VerticesObject::OnPaint(Shader* shader)
{
	Object::OnPaint(shader);
	glm::vec4 c = glm::vec4(DEFAULT_COLOR_VALUE);
	shader->setVec4("u_VertColor", c);
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

void ColoredVerticesObject::OnPaint(Shader* shader)
{
	VerticesObject::OnPaint(shader);
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

void IndexedColoredVerticesObject::OnPaint(Shader* shader)
{
	Object::OnPaint(shader);
	Renderer::Draw(*m_vao, *m_ebo);
}

class GameObject : public Object
{
public:
	virtual void OnPaint(Shader* shader);

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

void GameObject::OnPaint(Shader* shader)
{
	m_icvo->OnPaint(shader);
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
	virtual void OnPaint(Shader* shader);
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
	static void OnPaint(Shader* shader);
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

void SceneManager::OnPaint(Shader* shader)
{
	if (s_busying)
	{
		return;
	}

	if (s_activeScene != nullptr)
	{
		s_activeScene->OnPaint(shader);
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

void Scene::OnPaint(Shader* shader)
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

#pragma region Assignment given APIs

// screen setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// struct for storing the obj file
struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Model {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

Model loadOBJ(const char* objPath)
{
	// function to load the obj file
	// Note: this simple function cannot load all obj files.

	struct V {
		// struct for identify if a vertex has showed up
		unsigned int index_position, index_uv, index_normal;
		bool operator == (const V& v) const {
			return index_position == v.index_position && index_uv == v.index_uv && index_normal == v.index_normal;
		}
		bool operator < (const V& v) const {
			return (index_position < v.index_position) ||
				(index_position == v.index_position && index_uv < v.index_uv) ||
				(index_position == v.index_position && index_uv == v.index_uv && index_normal < v.index_normal);
		}
	};

	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::map<V, unsigned int> temp_vertices;

	Model model;
	unsigned int num_vertices = 0;

	std::cout << "\nLoading OBJ file " << objPath << "..." << std::endl;

	std::ifstream file;
	file.open(objPath);

	// Check for Error
	if (file.fail()) {
		std::cerr << "Impossible to open the file! Do you use the right path? See Tutorial 6 for details" << std::endl;
		exit(1);
	}

	while (!file.eof()) {
		// process the object file
		char lineHeader[128];
		file >> lineHeader;

		if (strcmp(lineHeader, "v") == 0) {
			// geometric vertices
			glm::vec3 position;
			file >> position.x >> position.y >> position.z;
			temp_positions.push_back(position);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			// texture coordinates
			glm::vec2 uv;
			file >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			// vertex normals
			glm::vec3 normal;
			file >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			// Face elements
			V vertices[3];
			for (int i = 0; i < 3; i++) {
				char ch;
				file >> vertices[i].index_position >> ch >> vertices[i].index_uv >> ch >> vertices[i].index_normal;
			}

			// Check if there are more than three vertices in one face.
			std::string redundency;
			std::getline(file, redundency);
			if (redundency.length() >= 5) {
				std::cerr << "There may exist some errors while load the obj file. Error content: [" << redundency << " ]" << std::endl;
				std::cerr << "Please note that we only support the faces drawing with triangles. There are more than three vertices in one face." << std::endl;
				std::cerr << "Your obj file can't be read properly by our simple parser :-( Try exporting with other options." << std::endl;
				exit(1);
			}

			for (int i = 0; i < 3; i++) {
				if (temp_vertices.find(vertices[i]) == temp_vertices.end()) {
					// the vertex never shows before
					Vertex vertex;
					vertex.position = temp_positions[vertices[i].index_position - 1];
					vertex.uv = temp_uvs[vertices[i].index_uv - 1];
					vertex.normal = temp_normals[vertices[i].index_normal - 1];

					model.vertices.push_back(vertex);
					model.indices.push_back(num_vertices);
					temp_vertices[vertices[i]] = num_vertices;
					num_vertices += 1;
				}
				else {
					// reuse the existing vertex
					unsigned int index = temp_vertices[vertices[i]];
					model.indices.push_back(index);
				}
			} // for
		} // else if
		else {
			// it's not a vertex, texture coordinate, normal or face
			char stupidBuffer[1024];
			file.getline(stupidBuffer, 1024);
		}
	}
	file.close();

	std::cout << "There are " << num_vertices << " vertices in the obj file.\n" << std::endl;
	return model;
}

#pragma endregion

#pragma region Assignment Specific Classes
#pragma region ModelObject
class ModelObject : Object
{
public:
	ModelObject();
	~ModelObject();

	void LoadModel(const char* path);

private:
	Model m_model;

};

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

#pragma region Main Scene
class MountainScene : Scene
{
public:
	MountainScene();
	~MountainScene();

	virtual void OnInitialize();
	virtual void OnPaint(Shader* shader);
	virtual void OnEnd();

private:
	FirstPersonPlayer* m_player;
};


MountainScene::MountainScene() : m_player(new FirstPersonPlayer())
{
}

MountainScene::~MountainScene()
{
	delete m_player;
}

void MountainScene::OnInitialize()
{
	m_player->SetActive(true);
}

void MountainScene::OnPaint(Shader* shader)
{
}

void MountainScene::OnEnd()
{
}

#pragma endregion

#pragma endregion

Shader* shader;
MountainScene* mountainScene;

void get_OpenGL_info()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

void sendDataToOpenGL()
{
	//TODO
	//Load objects and bind to VAO and VBO
	//Load textures
	mountainScene = new MountainScene();
}

void initializedGL(void) //run only once
{
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not OK." << std::endl;
	}

	get_OpenGL_info();
	sendDataToOpenGL();

	//TODO: set up the camera parameters	
	//TODO: set up the vertex shader and fragment shader
	shader = new Shader();
	shader->setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	shader->use();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void paintGL(void)  //always run
{
	//TODO:
	//Set lighting information, such as position and color of lighting source
	//Set transformation matrix
	//Bind different textures
	Renderer::Clear();
	Camera::OnPaint(shader);
	ObjectRenderPipeline::OnPaint(shader);
	SceneManager::OnPaint(shader);
}

void onEnd(void)
{
	delete shader;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Sets the mouse-button callback for the current window.	
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	// Sets the cursor position callback for the current window
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Sets the scoll callback for the current window.
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Sets the Keyboard callback for the current window.
}


int main(int argc, char* argv[])
{
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

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 2", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*register callback functions*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);                                                                  //    
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	initializedGL();

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	onEnd();

	glfwTerminate();

	return 0;
}