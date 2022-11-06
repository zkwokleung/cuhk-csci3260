#pragma once
#include "../myapi.h"
#include "Resources.h"

// Design the game logic here, and the application will handle the rendering
class Sandbox
{
public:
	Sandbox();
	~Sandbox();

	// Initialize everything about the game here
	void Initialize(void);

	// OnUpdate is called every loop
	void OnUpdate(void);

	// OnEnd is called before this sandbox shutdown
	void OnEnd(void);

private:
	Skybox* m_defaultSkybox;
};