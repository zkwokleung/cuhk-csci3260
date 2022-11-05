#include "callbacks.h"

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