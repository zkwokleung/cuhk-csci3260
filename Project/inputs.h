#pragma once
#include <glfw/glfw3.h>
#include <unordered_map>

typedef void(*KeyCallbackFunc)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void(*CursorPosCallbackFunc)(GLFWwindow* window, double xpos, double ypos);
typedef void(*MouseButtonCallbackFunc)(GLFWwindow* window, int button, int action, double xpos, int ypos);
typedef void(*ScrollCallbackFunc)(GLFWwindow* window, double xoffset, double yoffset);

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

class ScrollCallback
{
public:
	ScrollCallback(void);
	ScrollCallback(ScrollCallbackFunc func);
	void SetCallback(ScrollCallbackFunc func);

private:
	friend class Input;

	int m_id;
	ScrollCallbackFunc m_callback;
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

	// Scroll callback
	static void AddScrollCallback(ScrollCallback callback);
	static void RemoveScrollCallback(ScrollCallback callback);
	static void RemoveAllScrollCallbacks();

private:
	static unsigned int s_nextKey;
	static std::unordered_map<int, KeyCallback> s_keyCallbacks;
	static std::unordered_map<int, CursorPosCallback> s_cursorPosCallbacks;
	static std::unordered_map<int, MouseButtonCallback> s_mouseButtonCallbacks;
	static std::unordered_map<int, ScrollCallback> s_scrollCallbacks;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};