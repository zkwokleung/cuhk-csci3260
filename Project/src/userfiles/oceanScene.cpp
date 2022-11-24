#include "oceanScene.h"

OceanScene::OceanScene() : Scene(), m_player(new Player())
{
	// Initialize Skybox
	std::vector<std::string> faces = { "skybox/ocean/right.jpg",  "skybox/ocean/left.jpg",  "skybox/ocean/top.jpg",
									  "skybox/ocean/bottom.jpg", "skybox/ocean/front.jpg", "skybox/ocean/back.jpg" };

	GLuint cubemap = Resources::LoadCubemap(faces);
	m_skybox = new Skybox(cubemap, new Shader(Resources::LoadTextFile("shaders/SkyboxVert.glsl"),
		Resources::LoadTextFile("shaders/SkyboxFrag.glsl")));

	AddObject(m_player);
}

OceanScene::~OceanScene()
{
	delete m_skybox;
	delete m_player;
}

void OceanScene::OnInitialize()
{
	Scene::OnInitialize();

	Input::AddKeyCallback(this);

	// Enable skybox
	m_skybox->Enable();

	// InitializePlayer
	m_player->GetTransform().SetLocalPosition(glm::vec3());
	m_player->GetTransform().SetLocalRotation(glm::vec3());
	m_player->SetActive(true);
}

void OceanScene::OnUpdate(void)
{
	Scene::OnUpdate();

}

void OceanScene::OnPaint(Shader* shader)
{
}

void OceanScene::OnEnd()
{
	Scene::OnEnd();

	Input::RemoveKeyCallback(this);
}

void OceanScene::key_callback(unsigned char key, unsigned int action, int x, int y)
{
}
