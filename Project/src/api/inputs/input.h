#pragma once

// *** Change these lines to switch between GLFW and GLUT ***
//#define USE_GLFW
#define USE_GLUT

#ifdef USE_GLUT
#include <freeglut/freeglut.h>
#endif // USE_GLUT

#ifdef USE_GLFW
#include <glfw/glfw3.h>
#endif // USE_GLFW

#include <list>
#include <unordered_map>
#include <glm/glm.hpp>

// Macros for different keyboard actions
#define KEYBOARD_ACTION_DOWN 0x1
#define KEYBOARD_ACTION_PRESS 0x2
#define KEYBOARD_ACTION_RELEASE 0x3

#define KEYBOARD_KEYSTATE_NONE 0x0
#define KEYBOARD_KEYSTATE_PRESSED 0x1
#define KEYBOARD_KEYSTATE_PRESSINGS 0x2
#define KEYBOARD_KEYSTATE_RELEASE 0x3

#ifdef USE_GLFW
typedef void(*KeyCallbackFunc)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void(*CursorPosCallbackFunc)(GLFWwindow* window, double xpos, double ypos);
typedef void(*MouseButtonCallbackFunc)(GLFWwindow* window, int button, int action, double xpos, int ypos);
typedef void(*ScrollCallbackFunc)(GLFWwindow* window, double xoffset, double yoffset);
#endif // USE_GLFW

#ifdef USE_GLUT
// A event listener that listen to the input event
class IKeyCallback
{
public:
	virtual void key_callback(unsigned char key, unsigned int action, int x, int y) = 0;

private:
	// Allow the Input Manager to access the callback function
	friend class Input;
};

// A event listener that listen to the input event
class ICursorPosCallback
{
public:
	virtual void cursor_position_callback(int x, int y) = 0;

private:
	// Allow the Input Manager to access the callback function
	friend class Input;
};

// A event listener that listen to the input event
class IMouseButtonCallback
{
public:
	virtual void mouse_button_callbak(int button, int state, int x, int y) = 0;

private:
	// Allow the Input Manager to access the callback function
	friend class Input;
};

// A event listener that listen to the input event
class IScrollCallback
{
public:
	virtual void scroll_callback(int button, int dir, int x, int y) = 0;

private:
	// Allow the Input Manager to access the callback function
	friend class Input;
};
#endif // USE_GLUT

// The Input Manager to handle all the input events
class Input
{
public:
#ifdef USE_GLFW
	static void Init(GLFWwindow* window);
#endif // USE_GLFW
#ifdef USE_GLUT
	static void Init();
#endif // USE_GLUT

	// Keyboard keys callback
	static void AddKeyCallback(IKeyCallback* callback);
	static void RemoveKeyCallback(IKeyCallback* callback);
	static void RemoveAllKeyCallbacks();

	// Mouse button callback
	static void AddMouseButtonCallback(IMouseButtonCallback* callback);
	static void RemoveMouseButtonCallback(IMouseButtonCallback* callback);
	static void RemoveAllMouseButtonCallbacks();

	// Mouse move callback
	static void AddCursorPosCallback(ICursorPosCallback* callback);
	static void RemoveCursorPosCallback(ICursorPosCallback* callback);
	static void RemoveAllCursorPosCallbacks();

	// Scroll callback
	static void AddScrollCallback(IScrollCallback* callback);
	static void RemoveScrollCallback(IScrollCallback* callback);
	static void RemoveAllScrollCallbacks();

	// Lock the cursor at the center of the window
	static void SetCursorLock(bool lock);
	// Display or hide the cursor
	static void SetCursorDisplay(bool display);
	// Get the Position of the pointer
	static glm::vec2 GetCursorPos();

	// Called on each frame
	static void OnUpdate();

private:
	// Callbacks
	static std::list<IKeyCallback*> s_keyCallbacks;
	static std::list<ICursorPosCallback*> s_cursorPosCallbacks;
	static std::list<IMouseButtonCallback*> s_mouseButtonCallbacks;
	static std::list<IScrollCallback*> s_scrollCallbacks;

	// The state of each key, 0 is not pressed, 1 is currently pressed
	static std::unordered_map<unsigned char, int> s_keyStates;

	// The position of the cursor
	static glm::vec2 s_cursorPos;

	// Is the cursor locked at the center?
	static bool s_cursorLocked;

#ifdef USE_GLFW
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
#endif // USE_GLFW

#ifdef USE_GLUT
	static void glut_keyboard_callback(unsigned char key, int x, int y);
	static void glut_keyboard_up_callback(unsigned char key, int x, int y);
	static void glut_mouse_callback(int button, int state, int x, int y);
	static void glut_cursor_callback(int x, int y);
	static void glut_wheel_callback(int button, int dir, int x, int y);
#endif // USE_GLUT

};