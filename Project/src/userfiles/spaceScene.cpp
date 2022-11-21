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

	// Initialize Rocks
	Mesh* rockMesh = Resources::LoadObject("object/rock.obj");
	rockMesh->SetTexture(new Texture(Resources::LoadImageData("texture/rockTexture.bmp")));
	for (unsigned int i = 0; i < SPACE_ROCK_NUM; i++)
	{
		m_rocks[i] = new ModelObject(rockMesh);
	}

	// Initialize crafts
	Mesh* craftMesh = Resources::LoadObject("object/craft.obj");
	craftMesh->SetTexture(new Texture(Resources::LoadImageData("texture/vehicleTexture.bmp")));
	for (unsigned int i = 0; i < SPACE_CRAFT_NUM; i++)
	{
		m_spaceCrafts[i] = new ModelObject(craftMesh);

		// Initialize Lights
		m_craftLights[i] = new PointLight();
	}

	AddObject(m_player);
	AddObject(m_planet);
	for (unsigned int i = 0; i < SPACE_ROCK_NUM; i++)
	{
		AddObject(m_rocks[i]);
	}
	for (unsigned int i = 0; i < SPACE_CRAFT_NUM; i++)
	{
		AddObject(m_spaceCrafts[i]);
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
	// Enable skybox
	m_skybox->Enable();

	// InitializePlayer
	m_player->SetActive(true);

	// Initialize planet
	m_planet->GetTransform().SetLocalPosition(PLANET_INITIAL_POSITION);
	m_planet->GetTransform().SetLocalScale(glm::vec3(500.f));
	m_planet->GetTransform().SetLocalRotation(glm::vec3(90.f, .0f, .0f));
	m_planet->SetActive(true);

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
		// Random scale
		glm::vec3 scl = glm::vec3(Random::Range(SPACE_CRAFT_MIN_SCALE, SPACE_CRAFT_MAX_SCALE));
		m_spaceCrafts[i]->GetTransform().SetLocalScale(scl);

		m_spaceCrafts[i]->GetTransform().SetLocalPosition(
			glm::vec3(
				Random::Range(SPACE_CRAFT_MIN_POSITION_X, SPACE_CRAFT_MAX_POSITION_X),
				Random::Range(SPACE_CRAFT_MIN_POSITION_Y, SPACE_CRAFT_MAX_POSITION_Y),
				Random::Range(SPACE_CRAFT_MIN_POSITION_Z, SPACE_CRAFT_MAX_POSITION_Z)
			)
		);

		m_spaceCrafts[i]->SetActive(true);

		// Initialize Point Light for each Crafts
		m_craftLights[i]->GetTransform().SetParent(&m_spaceCrafts[i]->GetTransform());
		m_craftLights[i]->GetTransform().SetLocalPosition(glm::vec3(.0f, 2.f, .0f));
		m_craftLights[i]->SetAmbient(glm::vec3(
			Random::Range(0.f, 1.f),
			Random::Range(0.f, 1.f),
			Random::Range(0.f, 1.f)
		));
		m_craftLights[i]->SetPointLightParams(1.f, 0.0045f, 0.0001f);;
		m_craftLights[i]->SetActive(true);
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
	m_planet->GetTransform().SetLocalRotation(m_planet->GetTransform().GetLocalRotation() +
		glm::vec3(0.f, 0.f, 1.f) * Time::GetDeltaTime());

	// crafts self-rotate
	for (unsigned int i = 0; i < SPACE_CRAFT_NUM; i++)
	{

		static float countDown = 0;

		// Teleport the craft if times up
		if (countDown <= 0)
		{
			// Random location
			glm::vec3 newPos = glm::vec3(
				Random::Range(SPACE_CRAFT_MIN_POSITION_X, SPACE_CRAFT_MAX_POSITION_X),
				Random::Range(SPACE_CRAFT_MIN_POSITION_Y, SPACE_CRAFT_MAX_POSITION_Y),
				Random::Range(SPACE_CRAFT_MIN_POSITION_Z, SPACE_CRAFT_MAX_POSITION_Z)
			);
			m_spaceCrafts[i]->GetTransform().SetLocalPosition(newPos);

			countDown = SPACE_CRAFT_TELEPORT_INTERVAL;
		}
		else
		{
			// keep on counting
			countDown -= Time::GetDeltaTime();
		}

		// self-rotate
		m_spaceCrafts[i]->GetTransform().SetLocalRotation(m_spaceCrafts[i]->GetTransform().GetRotation() + SPACE_CRAFT_SELF_ROTATE_SPEED * Time::GetDeltaTime() * glm::vec3(0.f, 1.f, 0.f));
	}
}

void SpaceScene::OnPaint(Shader* shader)
{
}

void SpaceScene::OnEnd()
{
	Scene::OnEnd();
}