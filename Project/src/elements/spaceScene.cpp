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

	std::vector<ImageData*> cubemap = Resources::LoadCubemap(faces);
	m_skybox = new Skybox(cubemap, new Shader(
		Resources::LoadTextFile("shaders/SkyboxVert.glsl"),
		Resources::LoadTextFile("shaders/SkyboxFrag.glsl"))
	);

	// Free the cubemap data
	for (unsigned int i = 0; i < cubemap.size(); i++)
	{
		Resources::FreeImage(cubemap[i]);
		delete cubemap[i];
	}
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
