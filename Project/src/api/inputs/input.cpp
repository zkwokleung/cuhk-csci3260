#include "input.h"

unsigned int Input::s_nextKey = 0;
std::unordered_map<int, KeyCallback> Input::s_keyCallbacks;
std::unordered_map<int, CursorPosCallback> Input::s_cursorPosCallbacks;
std::unordered_map<int, MouseButtonCallback> Input::s_mouseButtonCallbacks;
std::unordered_map<int, ScrollCallback> Input::s_scrollCallbacks;

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