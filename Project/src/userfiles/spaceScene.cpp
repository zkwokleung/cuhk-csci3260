#include "spaceScene.h"

SpaceScene::SpaceScene() : m_player(new Player()), m_planetLight(new PointLight())
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

	faces =
	{
		"skybox/uv_mapper.jpg",
		"skybox/uv_mapper.jpg",
		"skybox/uv_mapper.jpg",
		"skybox/uv_mapper.jpg",
		"skybox/uv_mapper.jpg",
		"skybox/uv_mapper.jpg"
	};

	GLuint cubemap = Resources::LoadCubemap(faces);
	m_skybox = new Skybox(cubemap, new Shader(
		Resources::LoadTextFile("shaders/SkyboxVert.glsl"),
		Resources::LoadTextFile("shaders/SkyboxFrag.glsl"))
	);

	Mesh* planetMesh = Resources::LoadObject("object/planet.obj");
	planetMesh->SetTexture(new Texture(Resources::LoadImageData("texture/earthTexture.bmp")));
	m_planet = new ModelObject(planetMesh);
}

SpaceScene::~SpaceScene()
{
	delete m_skybox;
	delete m_player;
}

void SpaceScene::OnInitialize()
{
	// Enable skybox
	m_skybox->Enable();

	// InitializePlayer
	AddObject(m_player);
	m_player->SetActive(true);

	// Initialize planet
	AddObject(m_planet);
	m_planet->GetTransform().SetLocalPosition(glm::vec3(.0f, .0f, -1000.f));
	m_planet->GetTransform().SetLocalScale(glm::vec3(50.f));
	m_planet->GetTransform().SetLocalRotation(glm::vec3(90.f, .0f, .0f));
	m_planet->SetActive(true);

	AddObject(m_planetLight);
	m_planetLight->GetTransform().SetPosition(glm::vec3(.0f, .0f, -950.f));
	m_planetLight->SetPointLightParams(1.f, 0.045f, 0.0075f);
	m_planetLight->SetActive(true);
}

void SpaceScene::OnPaint(Shader* shader)
{

}

void SpaceScene::OnEnd()
{
}