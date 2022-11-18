#include "spaceScene.h"

SpaceScene::SpaceScene() : m_player(new Player()), m_planetLight(new PointLight()),
m_rocksContainer(new Object("rocks container"))
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

	GLuint cubemap = Resources::LoadCubemap(faces);
	m_skybox = new Skybox(cubemap, new Shader(
		Resources::LoadTextFile("shaders/SkyboxVert.glsl"),
		Resources::LoadTextFile("shaders/SkyboxFrag.glsl"))
	);

	Mesh* planetMesh = Resources::LoadObject("object/planet.obj");
	planetMesh->SetTexture(new Texture(Resources::LoadImageData("texture/earthTexture.bmp")));
	m_planet = new ModelObject(planetMesh);

	Mesh* rockMesh = Resources::LoadObject("object/rock.obj");
	rockMesh->SetTexture(new Texture(Resources::LoadImageData("texture/rockTexture.bmp")));
	for (unsigned int i = 0; i < SPACE_ROCK_NUM; i++)
	{
		m_rocks[i] = new ModelObject(rockMesh);
	}
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
	m_planet->GetTransform().SetLocalPosition(glm::vec3(1000.0f, .0f, -19000.f));
	m_planet->GetTransform().SetLocalScale(glm::vec3(500.f));
	m_planet->GetTransform().SetLocalRotation(glm::vec3(90.f, .0f, .0f));
	m_planet->SetActive(true);

	// Initialize rocks
	m_rocksContainer->GetTransform().SetParent(&m_planet->GetTransform());
	m_rocksContainer->GetTransform().SetLocalRotation(glm::vec3(100.f, 10.f, 190.f));
	m_rocksContainer->SetActive(true);
	for (unsigned int i = 0; i < SPACE_ROCK_NUM; i++)
	{
		AddObject(m_rocks[i]);

		// Randomize transform
		// Random radius distance to planet
		float distance = Random::Range(5.f, 10.f);

		// Random concentric rotation
		float angle = Random::Range(0.f, 360.f);
		glm::vec3 pos = distance * glm::vec3(glm::sin(glm::radians(angle)), 0.f, glm::cos(glm::radians(angle)));
		m_rocks[i]->GetTransform().SetLocalPosition(pos);

		// Random local rotation
		glm::vec3 rot = glm::vec3(Random::Range(0.f, 360.f), Random::Range(0.f, 360.f), Random::Range(0.f, 360.f));
		m_rocks[i]->GetTransform().SetLocalRotation(rot);

		// Random Scale
		glm::vec3 scl = glm::vec3(Random::Range(.1f, .2f), Random::Range(.1f, .2f), Random::Range(.1f, .2f));
		m_rocks[i]->GetTransform().SetLocalScale(scl);

		// Set parent to planet
		m_rocks[i]->GetTransform().SetParent(&m_rocksContainer->GetTransform());

		m_rocks[i]->SetActive(true);
	}

	// Initialize Light
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