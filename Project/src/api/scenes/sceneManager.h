#pragma once
#include "scene.h"

class SceneManager
{
public:
	static void OnInitialize();
	static void OnUpdate(void);
	static void OnPaint(Shader* shader);
	static void OnEnd();
	static void SetActiveScene(Scene* scene);
	static Scene* GetActiveScene();

private:
	static Scene* s_activeScene;
	static bool s_busying;
};