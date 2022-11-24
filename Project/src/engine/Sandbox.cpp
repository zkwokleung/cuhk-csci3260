#include "Sandbox.h"

// Design the game logic here, and the application will handle the rendering

Sandbox::Sandbox() : m_space(new SpaceScene()), m_ocean(new OceanScene())
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::Initialize(void)
{
	SceneManager::AddScene(m_space);
	SceneManager::AddScene(m_ocean);

	SceneManager::SetActiveScene(m_space);
}

void Sandbox::OnUpdate(void)
{
}

void Sandbox::OnEnd(void)
{
}
