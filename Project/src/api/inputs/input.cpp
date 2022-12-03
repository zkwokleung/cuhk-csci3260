#include "input.h"

glm::vec2 Input::s_cursorPos = glm::vec2(.0f);
bool Input::s_cursorLocked = false;
std::list<IKeyCallback* > Input::s_keyCallbacks;
std::list<ICursorPosCallback* > Input::s_cursorPosCallbacks;
std::list<IMouseButtonCallback* > Input::s_mouseButtonCallbacks;
std::list<IScrollCallback*> Input::s_scrollCallbacks;

#ifdef USE_GLFW
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

void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Invoke all the callback listeners
	for (unsigned int i = 0; i < s_scrollCallbacks.size(); i++)
	{
		if (s_scrollCallbacks[i].m_callback)
			s_scrollCallbacks[i].m_callback(window, xoffset, yoffset);
	}
}

// Initialize the Input Manager
void Input::Init(GLFWwindow* window)
{
	s_nextKey = 0;
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
}
#endif // USE_GLFW

#ifdef USE_GLUT
void Input::Init()
{
	// Keyboard
	glutKeyboardFunc(glut_keyboard_callback);
	glutKeyboardUpFunc(glut_keyboard_up_callback);
	glutSpecialFunc(glut_special_key_callback);
	glutSpecialUpFunc(glut_special_key_up_callback);

	// Mouse
	glutMouseFunc(glut_mouse_callback);
	glutMotionFunc(glut_cursor_callback);
	glutPassiveMotionFunc(glut_cursor_callback);
	glutMouseWheelFunc(glut_wheel_callback);
}

void Input::glut_keyboard_callback(unsigned char key, int x, int y)
{
	// Invoke all the callback listeners
	std::list<IKeyCallback*>::iterator it;
	for (it = s_keyCallbacks.begin(); it != s_keyCallbacks.end(); it++)
	{
		(*it)->key_callback(key, KEYBOARD_ACTION_PRESS, x, y);
	}
}

void Input::glut_keyboard_up_callback(unsigned char key, int x, int y)
{
	// Invoke all the callback listeners
	std::list<IKeyCallback*>::iterator it;
	for (it = s_keyCallbacks.begin(); it != s_keyCallbacks.end(); it++)
	{
		(*it)->key_callback(key, KEYBOARD_ACTION_RELEASE, x, y);
	}
}

void Input::glut_mouse_callback(int button, int state, int x, int y)
{
	// Invoke all the callback listeners
	std::list<IMouseButtonCallback*>::iterator it;
	for (it = s_mouseButtonCallbacks.begin(); it != s_mouseButtonCallbacks.end(); it++)
	{
		(*it)->mouse_button_callbak(button, state, x, y);
	}
}

void Input::glut_cursor_callback(int x, int y)
{
	if (s_cursorLocked && x == glutGet(GLUT_WINDOW_WIDTH) / 2 && y == glutGet(GLUT_WINDOW_HEIGHT) / 2)
		return;

	s_cursorPos = glm::vec2(x, y);
	// Invoke all the callback listeners
	std::list<ICursorPosCallback*>::iterator it;
	for (it = s_cursorPosCallbacks.begin(); it != s_cursorPosCallbacks.end(); it++)
	{
		(*it)->cursor_position_callback(x, y);
	}

	// Move the cursor back to the center of screen
	if (s_cursorLocked)
	{
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	}
}
void Input::glut_wheel_callback(int button, int dir, int x, int y)
{
	// Invoke all the callback listeners
	std::list<IScrollCallback*>::iterator it;
	for (it = s_scrollCallbacks.begin(); it != s_scrollCallbacks.end(); it++)
	{
		(*it)->scroll_callback(button, dir, x, y);
	}
}
void Input::glut_special_key_callback(int key, int x, int y)
{
	// Invoke all the callback listeners
	std::list<IKeyCallback*>::iterator it;
	for (it = s_keyCallbacks.begin(); it != s_keyCallbacks.end(); it++)
	{
		(*it)->key_callback(key, KEYBOARD_ACTION_PRESS, x, y);
	}
}
void Input::glut_special_key_up_callback(int key, int x, int y)
{
	// Invoke all the callback listeners
	std::list<IKeyCallback*>::iterator it;
	for (it = s_keyCallbacks.begin(); it != s_keyCallbacks.end(); it++)
	{
		(*it)->key_callback(key, KEYBOARD_ACTION_RELEASE, x, y);
	}
}
#endif // USE_GLUT

void Input::AddKeyCallback(IKeyCallback* callback)
{
	if (std::find(s_keyCallbacks.begin(), s_keyCallbacks.end(), callback) != s_keyCallbacks.end())
		return;

	s_keyCallbacks.push_back(callback);
}

void Input::RemoveKeyCallback(IKeyCallback* callback)
{
	if (std::find(s_keyCallbacks.begin(), s_keyCallbacks.end(), callback) == s_keyCallbacks.end())
		return;

	s_keyCallbacks.remove(callback);
}

void Input::RemoveAllKeyCallbacks()
{
	s_keyCallbacks.clear();
}

void Input::AddMouseButtonCallback(IMouseButtonCallback* callback)
{
	if (std::find(s_mouseButtonCallbacks.begin(), s_mouseButtonCallbacks.end(), callback) != s_mouseButtonCallbacks.end())
		return;

	s_mouseButtonCallbacks.push_back(callback);
}

void Input::RemoveMouseButtonCallback(IMouseButtonCallback* callback)
{
	if (std::find(s_mouseButtonCallbacks.begin(), s_mouseButtonCallbacks.end(), callback) == s_mouseButtonCallbacks.end())
		return;

	s_mouseButtonCallbacks.remove(callback);
}

void Input::RemoveAllMouseButtonCallbacks()
{
	s_mouseButtonCallbacks.clear();
}

void Input::AddCursorPosCallback(ICursorPosCallback* callback)
{
	if (std::find(s_cursorPosCallbacks.begin(), s_cursorPosCallbacks.end(), callback) != s_cursorPosCallbacks.end())
		return;

	s_cursorPosCallbacks.push_back(callback);
}

void Input::RemoveCursorPosCallback(ICursorPosCallback* callback)
{
	if (std::find(s_cursorPosCallbacks.begin(), s_cursorPosCallbacks.end(), callback) == s_cursorPosCallbacks.end())
		return;

	s_cursorPosCallbacks.remove(callback);
}

void Input::RemoveAllCursorPosCallbacks()
{
	s_cursorPosCallbacks.clear();
}

void Input::AddScrollCallback(IScrollCallback* callback)
{
	if (std::find(s_scrollCallbacks.begin(), s_scrollCallbacks.end(), callback) != s_scrollCallbacks.end())
		return;

	s_scrollCallbacks.push_back(callback);
}

void Input::RemoveScrollCallback(IScrollCallback* callback)
{
	if (std::find(s_scrollCallbacks.begin(), s_scrollCallbacks.end(), callback) == s_scrollCallbacks.end())
		return;

	s_scrollCallbacks.remove(callback);
}

void Input::RemoveAllScrollCallbacks()
{
	s_scrollCallbacks.clear();
}

void Input::SetCursorLock(bool lock)
{
	s_cursorLocked = lock;
}

void Input::SetCursorDisplay(bool display)
{
	if (display)
	{
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}
	else
	{
		glutSetCursor(GLUT_CURSOR_NONE);
	}
}

glm::vec2 Input::GetCursorPos()
{
	return s_cursorPos;
}

void Input::OnUpdate()
{
}