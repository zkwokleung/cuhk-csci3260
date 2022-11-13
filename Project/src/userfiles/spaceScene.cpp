#include "spaceScene.h"

SpaceScene::SpaceScene() : m_player(new Player()), m_fpp(new FirstPersonPlayer())
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

	//std::vector<std::string> faces =
	//{
	//	"skybox/uv_mapper.jpg",
	//	"skybox/uv_mapper.jpg",
	//	"skybox/uv_mapper.jpg",
	//	"skybox/uv_mapper.jpg",
	//	"skybox/uv_mapper.jpg",
	//	"skybox/uv_mapper.jpg"
	//};

	GLuint cubemap = Resources::LoadCubemap(faces);
	m_skybox = new Skybox(cubemap, new Shader(
		Resources::LoadTextFile("shaders/SkyboxVert.glsl"),
		Resources::LoadTextFile("shaders/SkyboxFrag.glsl"))
	);

	Mesh* rockMesh = Resources::LoadObject("object/rock.obj");
	rockMesh->SetTexture(new Texture(Resources::LoadImageData("texture/rockTexture.bmp")));
	m_rock = new ModelObject(rockMesh);
}

SpaceScene::~SpaceScene()
{
	delete m_skybox;
	delete m_player;
}

void SpaceScene::OnInitialize()
{
	Input::AddCursorPosCallback(cursor_position_callback);

	m_skybox->Enable();
	AddObject(m_player);
	//m_player->SetActive(true);

	AddObject(m_fpp);
	m_fpp->SetActive(true);

	AddObject(m_rock);
	m_rock->GetTransform().SetLocalPosition(glm::vec3(.0f, .0f, 10.f));
	m_rock->SetActive(true);
}

void SpaceScene::OnPaint(Shader* shader)
{

}

void SpaceScene::OnEnd()
{
}


void SpaceScene::cursor_position_callback(int x, int y)
{
	static glm::vec2 lastPos = glm::vec2(.0f);

	// Get Main Camera
	Camera* cam = Camera::GetMain();

	// The speed of the camera movement
	static float speed = .1f;

	// handle the mouse input
	glm::vec2 newPos = glm::vec2(x, y);
	glm::vec2 deltaPos = newPos - lastPos;

	// Up down
	glm::vec3 cameraRotation = cam->GetTransform().GetRotation();
	cameraRotation += glm::vec3(deltaPos.y * speed, .0f, .0f);

	// Left right
	cameraRotation += glm::vec3(.0f, deltaPos.x * speed, .0f);

	// Clamp the rotation value
	if (cameraRotation.x >= 360.f || cameraRotation.x <= -360.f)
		cameraRotation.x = .0f;
	if (cameraRotation.y >= 360.f || cameraRotation.y <= -360.f)
		cameraRotation.y = .0f;

	// Set the rotation of the camera
	cam->GetTransform().SetRotation(cameraRotation);

	lastPos = glm::vec2(x, y);
}