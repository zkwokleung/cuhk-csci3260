#include "Sandbox.h"

// Design the game logic here, and the application will handle the rendering

Sandbox::Sandbox() : m_scene(new SpaceScene())
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::Initialize(void)
{
	SceneManager::SetActiveScene(m_scene);
}

void Sandbox::OnUpdate(void)
{
}

void Sandbox::OnEnd(void)
{
}
