#include "oceanScene.h"

OceanScene::OceanScene() : Scene(), m_player(new Player())
{
	// Initialize Skybox
	std::vector<std::string> faces = { "skybox/ocean/right.jpg",  "skybox/ocean/left.jpg",  "skybox/ocean/top.jpg",
									  "skybox/ocean/bottom.jpg", "skybox/ocean/front.jpg", "skybox/ocean/back.jpg" };

	GLuint cubemap = Resources::LoadCubemap(faces);
	m_skybox = new Skybox(cubemap, new Shader(Resources::LoadTextFile("shaders/SkyboxVert.glsl"),
		Resources::LoadTextFile("shaders/SkyboxFrag.glsl")));

	// Initialize terrain
	Model* terrainModel = Resources::LoadObject("object/MountainTerrain.obj");
	terrainModel->SetTexture(new Texture(Resources::LoadImageData("texture/soil.jpg")));
	m_terrain = new ModelObject(terrainModel);

	AddObject(m_player);
	AddObject(m_terrain);
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
	m_player->GetTransform().SetLocalRotation(glm::vec3(0.0f, .0f, .0f));
	m_player->SetActive(true);

	// Initialize terrain
	m_terrain->GetTransform().SetLocalPosition(glm::vec3(.0f, -10000.f, .0f));
	m_terrain->GetTransform().SetLocalScale(glm::vec3(1000.f));
	m_terrain->SetActive(true);
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
