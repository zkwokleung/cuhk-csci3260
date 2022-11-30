#include "oceanScene.h"

OceanScene::OceanScene() : Scene(), m_player(new Player()), m_dirLight(new DirectionalLight())
{
	// Initialize Skybox
	std::vector<std::string> faces = { "skybox/ocean/right.jpg",  "skybox/ocean/left.jpg",  "skybox/ocean/top.jpg",
									  "skybox/ocean/bottom.jpg", "skybox/ocean/front.jpg", "skybox/ocean/back.jpg" };

	GLuint cubemap = Resources::LoadCubemap(faces);
	m_skybox = new Skybox(cubemap, new Shader(Resources::LoadTextFile("shaders/SkyboxVert.glsl"),
		Resources::LoadTextFile("shaders/SkyboxFrag.glsl")));

	// Initialize ground
	Model* groundModel = Resources::LoadObject("object/ground.obj");
	groundModel->SetTexture(new Texture(Resources::LoadImageData("texture/water.jpg")));
	m_ground = new ModelObject(groundModel);
	AddObject(m_ground);

	// Initialzie mountains
	for (unsigned int i = 0; i < MOUNTAIN_COUNT; i++)
	{
		m_mountains[i] = new Mountain();
		AddObject(m_mountains[i]);
	}

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

	// Initialize directional light
	m_dirLight->SetAmbient(glm::vec3(1.f));
	m_dirLight->SetIntensity(.5f);
	m_dirLight->SetDirection(glm::vec3(-1.f, -1.f, .0f));
	m_dirLight->SetActive(true);

	// InitializePlayer
	m_player->GetTransform().SetLocalPosition(glm::vec3());
	m_player->GetTransform().SetLocalRotation(glm::vec3(0.0f, 1000.0f, .0f));
	m_player->SetActive(true);

	// Initialize ground
	m_ground->GetTransform().SetLocalPosition(glm::vec3(.0f, -11000.f, .0f));
	m_ground->GetTransform().SetLocalScale(glm::vec3(
		100000.f,
		1.f,
		100000.f
	));
	m_ground->SetActive(true);

	// Intialize Mountains
	float xPos = -10000.f;
	float zPos = 80000.f;
	for (unsigned int i = 0; i < MOUNTAIN_COUNT; i++)
	{
		m_mountains[i]->GetTransform().SetLocalPosition(glm::vec3(
			xPos,
			-9000.f,
			zPos
		));
		xPos += 30000.f;
		zPos *= -1;
		float scale = Random::Range(MOUNTAIN_SIZE_MIN, MOUNTAIN_SIZE_MAX);
		m_mountains[i]->GetTransform().SetLocalScale(glm::vec3(scale, scale, scale));

		m_mountains[i]->SetActive(true);
	}
}

void OceanScene::OnUpdate(void)
{
	Scene::OnUpdate();

}

void OceanScene::OnPaint(Shader* shader)
{
	Scene::OnPaint(shader);
}

void OceanScene::OnEnd()
{
	Scene::OnEnd();

	Input::RemoveKeyCallback(this);
}

void OceanScene::key_callback(unsigned char key, unsigned int action, int x, int y)
{
}
