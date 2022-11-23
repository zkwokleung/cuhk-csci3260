#include "spaceScene.h"

SpaceScene::SpaceScene()
	: m_player(new Player()),
	m_planetLight(new PointLight(glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), 1, .007f, .008f)),
	m_rocksContainer(new Object("rocks container"))
{
	// Initialize Skybox
	std::vector<std::string> faces = { "skybox/right.bmp",  "skybox/left.bmp",  "skybox/top.bmp",
									  "skybox/bottom.bmp", "skybox/front.bmp", "skybox/back.bmp" };

	GLuint cubemap = Resources::LoadCubemap(faces);
	m_skybox = new Skybox(cubemap, new Shader(Resources::LoadTextFile("shaders/SkyboxVert.glsl"),
		Resources::LoadTextFile("shaders/SkyboxFrag.glsl")));

	// Initialize Planet
	Mesh* planetMesh = Resources::LoadObject("object/planet.obj");
	planetMesh->SetTexture(new Texture(Resources::LoadImageData("texture/earthTexture.bmp")));
	planetMesh->SetNormalMap(new Texture(Resources::LoadImageData("texture/earthNormal.bmp")));
	m_planet = new ModelObject(planetMesh);
	m_planet->SetName("planet");

	// Initialize Rocks
	Mesh* rockMesh = Resources::LoadObject("object/rock.obj");
	rockMesh->SetTexture(new Texture(Resources::LoadImageData("texture/rockTexture.bmp")));
	for (unsigned int i = 0; i < SPACE_ROCK_NUM; i++)
	{
		m_rocks[i] = new ModelObject(rockMesh);
		AddObject(m_rocks[i]);
	}

	// Initialize crafts
	for (unsigned int i = 0; i < SPACE_CRAFT_NUM; i++)
	{
		m_spaceCrafts[i] = new Craft();
		AddObject(m_spaceCrafts[i]);
	}

	AddObject(m_player);
	AddObject(m_planet);
	for (unsigned int i = 0; i < SPACE_ROCK_NUM; i++)
	{
	}
	AddObject(m_planetLight);
}

SpaceScene::~SpaceScene()
{
	delete m_skybox;
	delete m_player;
}

void SpaceScene::OnInitialize()
{
	Input::AddKeyCallback(this);

	// Enable skybox
	m_skybox->Enable();

	// InitializePlayer
	m_player->GetTransform().SetLocalPosition(glm::vec3());
	m_player->GetTransform().SetLocalRotation(glm::vec3());
	m_player->SetActive(true);

	// Initialize planet
	m_planet->GetTransform().SetLocalPosition(PLANET_INITIAL_POSITION);
	m_planet->GetTransform().SetLocalScale(glm::vec3(500.f));
	m_planet->GetTransform().SetLocalRotation(glm::vec3(90.f, .0f, .0f));
	m_planet->SetActive(true);
	SphereCollider* planetCollider = new SphereCollider(50000.f);
	m_planet->AddComponent(planetCollider);
	planetCollider->SetEnabled(true);

	// Initialize rocks
	m_rocksContainer->GetTransform().SetParent(&m_planet->GetTransform());
	m_rocksContainer->GetTransform().SetLocalRotation(glm::vec3(100.f, 10.f, 190.f));
	m_rocksContainer->SetActive(true);
	for (unsigned int i = 0; i < SPACE_ROCK_NUM; i++)
	{
		// Randomize transform
		// Random radius distance to planet
		float distance = Random::Range(SPACE_ROCK_MIN_DISTANCE, SPACE_ROCK_MAX_DISTANCE);

		// Random concentric rotation
		float angle = Random::Range(0.f, 360.f);
		glm::vec3 pos = distance * glm::vec3(glm::sin(glm::radians(angle)), 0.f, glm::cos(glm::radians(angle)));
		m_rocks[i]->GetTransform().SetLocalPosition(pos);

		// Random local rotation
		glm::vec3 rot = glm::vec3(Random::Range(0.f, 360.f), Random::Range(0.f, 360.f), Random::Range(0.f, 360.f));
		m_rocks[i]->GetTransform().SetLocalRotation(rot);

		// Random Scale
		glm::vec3 scl = glm::vec3(Random::Range(SPACE_ROCK_MIN_SCALE, SPACE_ROCK_MAX_SCALE),
			Random::Range(SPACE_ROCK_MIN_SCALE, SPACE_ROCK_MAX_SCALE),
			Random::Range(SPACE_ROCK_MIN_SCALE, SPACE_ROCK_MAX_SCALE));
		m_rocks[i]->GetTransform().SetLocalScale(scl);

		// Set parent to planet
		m_rocks[i]->GetTransform().SetParent(&m_rocksContainer->GetTransform());

		m_rocks[i]->SetActive(true);
	}

	// Initialize space crafts
	for (unsigned int i = 0; i < SPACE_CRAFT_NUM; i++)
	{
		m_spaceCrafts[i]->SetActive(true);
	}

	// Initialize Light
	m_planetLight->GetTransform().SetLocalPosition(PLANET_INITIAL_POSITION + glm::vec3(1000.0f, 1000.f, 1000.f));
	m_planetLight->SetAmbient(glm::vec3(1.f, 1.f, .5f));
	m_planetLight->SetPointLightParams(1.f, 0.0000045f, 0.00000025f);
	m_planetLight->SetActive(true);
}

void SpaceScene::OnUpdate(void)
{
	// Planet self-rotate
	m_planet->GetTransform().Rotate(glm::vec3(0.f, 10.f, .0f) * Time::GetDeltaTime());
}

void SpaceScene::OnPaint(Shader* shader)
{
}

void SpaceScene::OnEnd()
{
	Scene::OnEnd();
}

void SpaceScene::key_callback(unsigned char key, unsigned int action, int x, int y)
{
	if (key == 'r' && action == KEYBOARD_ACTION_PRESS)
	{
		// Reset the scene
		OnInitialize();
	}
}
