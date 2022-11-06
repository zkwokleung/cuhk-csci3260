#include "spaceScene.h"

SpaceScene::SpaceScene() : m_player(new Player())
{
	std::vector<std::string> faces =
	{
		"skybox/right.bmp",
		"skybox/left.bmp",
		"skybox/top.bmp",
		"skybox/bottom.bmp",
		"skybox/front.bmp",
		"skybox/back.bmp"
	};
	m_skybox = new Skybox(faces, new Shader(
		Resources::LoadTextFile("shaders/SkyboxVert.glsl"),
		Resources::LoadTextFile("shaders/SkyboxFrag.glsl"))
	);
}

SpaceScene::~SpaceScene()
{
	delete m_skybox;
	delete m_player;
}

void SpaceScene::OnInitialize()
{
	m_skybox->Enable();
	AddObject(m_player);
	m_player->SetActive(true);
}

void SpaceScene::OnPaint(Shader* shader)
{
}

void SpaceScene::OnEnd()
{
}
