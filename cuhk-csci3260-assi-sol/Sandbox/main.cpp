#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <list>


GLuint programID;
float x_delta = 0.1f;
float y_rotate_delta = 1.f;
float y_rotation = 0;
int x_press_num = 0;

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

	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 value);

	glm::vec3 GetRotation() const;
	void SetRotation(glm::vec3 value);

	glm::vec3 GetScale() const;
	void SetScale(glm::vec3 value);
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

glm::vec3 Transform::GetLeft() const
{
	return glm::vec3(
		glm::cos(glm::radians(GetRotation().y)),
		0,
		glm::sin(glm::radians(GetRotation().y))
	);
}

glm::vec3 Transform::GetRight() const
{
	return -1.f * GetLeft();
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

#pragma endregion

VAO* vaoTri;
VBO* vboTri;
EBO* eboTri;
Camera* mainCamera;

IndexedColoredVerticesObject* triangleObject;

void get_OpenGL_info() {
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void sendDataToOpenGL() {
	mainCamera = new Camera();
	mainCamera->GetTransform().SetPosition(glm::vec3(.0f, .0f, 10.f));
	Camera::SetMain(mainCamera);

	const GLfloat triangle[] =
	{
		-0.5f, -0.5f, +0.0f,  // left
		+0.0f, +0.0f, +1.0f,  // color

		+0.5f, -0.5f, +0.0f,  // right
		+0.0f, +1.0f, +1.0f,

		+0.0f, +0.5f, +0.0f,  // top
		+1.0f, +0.0f, +1.0f,
	};

	//GLuint vaoID;
	//glGenVertexArrays(1, &vaoID);
	//glBindVertexArray(vaoID);  //first VAO

	//GLuint vboID;
	//glGenBuffers(1, &vboID);
	//glBindBuffer(GL_ARRAY_BUFFER, vboID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	//// vertex position
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//// vertex color
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	//// with indexing (uncomment to use)
	GLuint indices[] = { 0, 1, 2 };
	//// index buffer
	//GLuint indexBufferID;
	//glGenBuffers(1, &indexBufferID);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	vaoTri = new VAO();
	vboTri = new VBO(triangle, 18 * sizeof(float));
	vaoTri->LinkAttrib(*vboTri, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);
	vaoTri->LinkAttrib(*vboTri, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	eboTri = new EBO(indices, 3);

	triangleObject = new IndexedColoredVerticesObject(triangle, 18, indices, 3);
	triangleObject->SetActive(true);
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

bool checkShaderStatus(GLuint shaderID) {
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
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
	glUseProgram(programID);

}

void initializedGL(void) {
	// run only once
	sendDataToOpenGL();
	installShaders();
}

void paintGL(void) {
	// always run
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  //specify the background color
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrix = glm::translate(glm::mat4(1.0f),
		glm::vec3(x_delta * x_press_num, 0.0f, 0.0f));;
	modelTransformMatrix = glm::scale(modelTransformMatrix, glm::vec3(5.0f, 5.0f, 5.0f));

	//GLint modelTransformMatrixUniformLocation =
	//	glGetUniformLocation(programID, "u_modelMatrix");
	//glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
	//	GL_FALSE, &modelTransformMatrix[0][0]);

	triangleObject->GetTransform().SetPosition(glm::vec3(x_delta * x_press_num, 0.0f, 0.0f));
	triangleObject->GetTransform().SetScale(glm::vec3(1.0f, 5.0f, 5.0f));

	Camera::OnPaint();
	ObjectRenderPipeline::OnPaint();

	//Renderer::Draw(*vaoTri, *eboTri);

	// glBindVertexArray();

	// without indexing
	//glDrawArrays(GL_TRIANGLES, 0, 6);  //render primitives from array data
	//// with indexing (uncomment to use)
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

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
	window = glfwCreateWindow(512, 512, "Hello Triangle", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

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