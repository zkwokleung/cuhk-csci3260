#include "input.h"

unsigned int Input::s_nextKey = 0;
glm::vec2 Input::s_cursorPos = glm::vec2(.0f);
bool Input::s_cursorLocked = false;
std::unordered_map<int, KeyCallback> Input::s_keyCallbacks;
std::unordered_map<int, CursorPosCallback> Input::s_cursorPosCallbacks;
std::unordered_map<int, MouseButtonCallback> Input::s_mouseButtonCallbacks;
std::unordered_map<int, ScrollCallback> Input::s_scrollCallbacks;

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

	// Mouse
	glutMouseFunc(glut_mouse_callback);
	glutMotionFunc(glut_cursor_callback);
	glutPassiveMotionFunc(glut_cursor_callback);
	glutMouseWheelFunc(glut_wheel_callback);
}

void Input::glut_keyboard_callback(unsigned char key, int x, int y)
{
	// Invoke all the callback listeners
	for (unsigned int i = 0; i < s_keyCallbacks.size(); i++)
	{
		if (s_keyCallbacks[i].m_callback)
			s_keyCallbacks[i].m_callback(key, KEYBOARD_ACTION_PRESS, x, y);
	}
}

void Input::glut_keyboard_up_callback(unsigned char key, int x, int y)
{
	// Invoke all the callback listeners
	for (unsigned int i = 0; i < s_keyCallbacks.size(); i++)
	{
		if (s_keyCallbacks[i].m_callback)
			s_keyCallbacks[i].m_callback(key, KEYBOARD_ACTION_RELEASE, x, y);
	}
}

void Input::glut_mouse_callback(int button, int state, int x, int y)
{
	// Invoke all the callback listeners
	for (unsigned int i = 0; i < s_mouseButtonCallbacks.size(); i++)
	{
		if (s_mouseButtonCallbacks[i].m_callback)
			s_mouseButtonCallbacks[i].m_callback(button, state, x, y);
	}
}

void Input::glut_cursor_callback(int x, int y)
{
	s_cursorPos = glm::vec2(x, y);
	// Invoke all the callback listeners
	for (unsigned int i = 0; i < s_cursorPosCallbacks.size(); i++)
	{
		if (s_cursorPosCallbacks[i].m_callback)
			s_cursorPosCallbacks[i].m_callback(x, y);
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
	for (unsigned int i = 0; i < s_scrollCallbacks.size(); i++)
	{
		if (s_scrollCallbacks[i].m_callback)
			s_scrollCallbacks[i].m_callback(button, dir, x, y);
	}
}
#endif // USE_GLUT

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

void Input::AddScrollCallback(ScrollCallback callback)
{
	if (callback.m_id != -1)
		return;

	ScrollCallback newCallback(callback);
	s_scrollCallbacks[s_nextKey] = newCallback;
	callback.m_id = s_nextKey;
	s_nextKey++;
}

void Input::RemoveScrollCallback(ScrollCallback callback)
{
	if (callback.m_id == -1)
		return;

	s_scrollCallbacks.erase(callback.m_id);
	callback.m_id = -1;
}

void Input::RemoveAllScrollCallbacks()
{
	while (s_scrollCallbacks.size() > 0)
	{
		RemoveScrollCallback(s_scrollCallbacks.at(0));
	}
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

ScrollCallback::ScrollCallback(void) : m_id(-1), m_callback(nullptr)
{
}

ScrollCallback::ScrollCallback(ScrollCallbackFunc func) : m_id(-1)
{
	SetCallback(func);
}

void ScrollCallback::SetCallback(ScrollCallbackFunc func)
{
	m_callback = func;
}