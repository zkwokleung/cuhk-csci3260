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

#pragma region My Classes
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
	VBO(GLfloat* vertices, GLsizeiptr size);
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
	void LinkVBO(VBO& VBO, GLuint layout);

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
	EBO(GLuint* indices, GLuint count);
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

VBO::VBO() {
}

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	std::cout << "Buffer ID before: " << ID << std::endl;
	glGenBuffers(1, &ID);
	std::cout << "Buffer ID: " << ID << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
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
	glDeleteBuffers(1, &ID);
}

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

VAO::~VAO()
{
	Delete();
}

void VAO::LinkVBO(VBO& VBO, GLuint layout)
{
	VBO.Bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
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
	glDeleteVertexArrays(1, &ID);
}

EBO::EBO()
{
	Count = 0;
	glGenBuffers(1, &ID);
}

EBO::EBO(GLuint* indices, GLuint count)
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
	glDeleteBuffers(1, &ID);
}

void Renderer::Clear()
{
	glClearColor(.07f, .13f, .17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
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
	glDrawElements(GL_TRIANGLES, ebo.Count, GL_UNSIGNED_INT, nullptr);
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
	Camera related
***********************/
class Camera
{
public:
	static glm::mat4 GetViewMatrix();
	static glm::mat4 GetProjectionMatrix();

	static glm::vec3 GetPosition();
	static void SetPosition(glm::vec3 pos);

	static void OnPaint();
private:
	static glm::vec3 m_position;
};

glm::vec3 Camera::m_position = glm::vec3(0.0f, 0.0f, 0.0f);

glm::mat4 Camera::GetViewMatrix()
{
	return glm::translate(glm::mat4(1.0f), -1.0f * GetPosition());
}

glm::mat4 Camera::GetProjectionMatrix()
{
	//return glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

glm::vec3 Camera::GetPosition()
{
	return m_position;
}

void Camera::SetPosition(glm::vec3 pos)
{
	m_position = pos;
}

void Camera::OnPaint()
{
	// Set the shader's projection and view uniform
	SetUniformMat4f("u_viewMatrix", GetViewMatrix());
	SetUniformMat4f("u_projectionMatrix", GetProjectionMatrix());
}

/**********************
	Game Object related
***********************/
class Transform
{
public:
	Transform();
	~Transform();

	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 value);

	glm::vec3 GetRotation();
	void SetRotation(glm::vec3 value);

	glm::vec3 GetScale();
	void SetScale(glm::vec3 value);

	void OnPaint();
	glm::mat4 GetTransformMat4();
private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};

#define DEFAULT_COLOR_VALUE 0.8f, 0.8f, 0.8f, 1.0f

class Object
{
public:
	Object();
	~Object();

	void SetVertices(GLfloat vertices[], int vertCount);
	void SetIndices(GLuint indices[], int idxCount);
	void SetVerticesColor(GLfloat colors[]);

	Transform GetTransform() const;

	void SetActive(bool active);
	bool IsActive() const;

	void OnPaint();

private:
	Transform m_transform;
	VAO m_vao;
	VBO m_vertVBO, m_colorVBO;
	EBO m_ebo;
	int m_verticesCount;
	bool m_useEBO;
	bool m_useColorVBO;
	bool m_isActive;
	bool m_ready;
};

class ObjectRenderPipeline
{
public:
	static void AddObject(Object& object);
	static void RemoveObject(Object& object);
	static void OnPaint();
private:
	static std::list<Object*> m_Objects;
};

Transform::Transform():
	m_position(glm::vec3()), m_rotation(glm::vec3()), m_scale(glm::vec3(1.f))
{}

Transform::~Transform()
{
}

glm::vec3 Transform::GetPosition()
{
	return m_position;
}

void Transform::SetPosition(glm::vec3 value)
{
	m_position = value;
}

glm::vec3 Transform::GetRotation()
{
	return m_rotation;
}

void Transform::SetRotation(glm::vec3 value)
{
	m_rotation = value;
}

glm::vec3 Transform::GetScale()
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

glm::mat4 Transform::GetTransformMat4()
{
	glm::mat4 t = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 r_x = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 r_y = glm::rotate(glm::mat4(1.0f), m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 r_z = glm::rotate(glm::mat4(1.0f), m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 s = glm::scale(glm::mat4(1.0f), m_scale);
	return s * r_z * r_y * r_x * t;
}

Object::Object() :
	m_vao(), m_vertVBO(), m_verticesCount(0), m_colorVBO(), m_transform(Transform()),
	m_isActive(false), m_useColorVBO(false), m_useEBO(false), m_ready(false)
{
}

Object::~Object()
{
	m_colorVBO.Delete();
	m_vertVBO.Delete();
	m_vao.Delete();
	m_ebo.Delete();
}

void Object::SetVertices(GLfloat vertices[], int vertCount)
{
	m_vertVBO = VBO(vertices, vertCount * sizeof(float));
	m_vao.LinkVBO(m_vertVBO, 0);

	m_verticesCount = vertCount;
	m_ready = true;
}

void Object::SetIndices(GLuint indices[], int idxCount)
{
	m_ebo = EBO(indices, idxCount);

	m_useEBO = true;
}

void Object::SetVerticesColor(GLfloat colors[])
{
	if (m_verticesCount < 1)
		m_verticesCount = sizeof(colors) / sizeof(float);

	m_colorVBO = VBO(colors, m_verticesCount * sizeof(float));
	m_vao.LinkVBO(m_colorVBO, 1);
}

Transform Object::GetTransform() const
{
	return m_transform;
}

void Object::SetActive(bool active)
{
	if (active)
	{
		ObjectRenderPipeline::AddObject(*this);
	}
	else
	{
		ObjectRenderPipeline::RemoveObject(*this);
	}

	m_isActive = active;
}

bool Object::IsActive() const
{
	return m_isActive;
}

void Object::OnPaint()
{
	if (!m_ready)
		return;

	m_transform.OnPaint();

	if (!m_useColorVBO) {
		SetUniform4f("u_Color", DEFAULT_COLOR_VALUE);
	}

	if (!m_useEBO)
	{
		Renderer::Draw(m_vao, GL_TRIANGLES, 6);
	}
	else
	{
		Renderer::Draw(m_vao, m_ebo);
	}
}

std::list<Object*> ObjectRenderPipeline::m_Objects;

void ObjectRenderPipeline::AddObject(Object& object)
{
	if (object.IsActive())
		return;
	m_Objects.push_back(&object);
}

void ObjectRenderPipeline::RemoveObject(Object& object)
{
	if (!object.IsActive())
		return;
	m_Objects.remove(&object);
}

void ObjectRenderPipeline::OnPaint()
{
	if (m_Objects.size() < 1)
		return;

	for (std::list<Object*>::iterator it = m_Objects.begin(); it != m_Objects.end(); it++)
	{
		(*it)->OnPaint();
		/*std::cout << "Painting Transform:" << std::endl;
		glm::mat4 t = (*it)->GetTransform().GetTransformMat4();
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				std::cout << t[j][i] << " ";
			}
			std::cout << std::endl;
		}*/
	}
}
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


Object* pyramid;
Object* ground;

void sendDataToOpenGL() {
	// TODO:
	// create 3D objects and/or 2D objects and/or lines (points) here and bind to VAOs & VBOs
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
	GLuint idxCube[] =
	{
		0, 1, 2,
		0, 4, 2,
		1,
	};

	GLfloat vertPyramid[] =
	{
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
	};

	GLfloat colorPyramid[] =
	{
		0.0f, 0.0f, 0.0f,
		0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,1.0f,
		1.0f,0.0f,0.0f
	};

	GLuint idxPyramid[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
		1, 2, 3,
		3, 4, 1
	};

	GLfloat vertGround[] =
	{
		-1.0f, +0.0f, -1.0f,
		-1.0f, +0.0f, +1.0f,
		+1.0f, +0.0f, -1.0f,
		-1.0f, +0.0f, +1.0f,
		+1.0f, +0.0f, +1.0f,
		+1.0f, +0.0f, -1.0f,
	};

	GLfloat colorGround[] =
	{
		+0.2f, +0.2f, +0.3f,
		+0.52f, +0.37f, +0.26f,
		+0.2f, +0.2f, +0.3f,
		+0.52f, +0.37f, +0.26f,
		+0.52f, +0.37f, +0.26f,
		+0.2f, +0.2f, +0.3f,
	};

	pyramid = new Object();
	pyramid->SetVertices(vertPyramid, 15);
	pyramid->SetIndices(idxPyramid, 18);
	pyramid->SetVerticesColor(colorPyramid);
	pyramid->GetTransform().SetScale(glm::vec3(1.0f, 1.0f, 1.f));
	pyramid->GetTransform().SetPosition(glm::vec3(10.0f, 10.0f, -10.f));
	pyramid->SetActive(true);

	//ground = new Object();
	//ground->SetVertices(vertGround, 18);
	//ground->SetVerticesColor(colorGround);
	//ground->GetTransform().SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	//ground->SetActive(true);
}

void paintGL(void) {
	// always run
	// TODO:
	// render your objects and control the transformation here
	Renderer::Clear();
	Camera::OnPaint();
	ObjectRenderPipeline::OnPaint();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO:

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void initializedGL(void) {
	// run only once
	// TODO:
	glEnable(GL_DEPTH_TEST);
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
