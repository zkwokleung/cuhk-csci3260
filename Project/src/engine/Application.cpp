#include "Application.h"
#include "Resources.h"
#include "Sandbox.h"
#include "Time.h"

// screen setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

Sandbox* Application::s_activeSandbox = nullptr;
Shader* Application::s_defaultShader = nullptr;

void Application::Initialize(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Project");
	glutDisplayFunc(MainLoop);

	// Initialize random seed
	srand(time(NULL));

	// Setup OpenGL
	glewInit();
	GetOpenGLInfo();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void Application::Run(void)
{
	Setup();
	glutMainLoop();
	End();
}

void Application::Setup(void)
{
	s_activeSandbox = new Sandbox();

	// Initialize fields and API
	Debug::Init();
	Random::Init();
	Input::Init();
	Skybox::Init();
	PhysicsEngine::Init();
	InstallShader();
	Renderer::SetClearColor(glm::vec4(1.f));

	s_activeSandbox->Initialize();
}

void Application::MainLoop(void)
{
	Time::OnFrameStart();

	// Call sandbox's OnUpdate
	s_activeSandbox->OnUpdate();

	// Handle Physics
	PhysicsEngine::OnUpdate();

	// Update the game
	SceneManager::OnUpdate();
	Camera::Update();
	ObjectRenderPipeline::OnUpdate();

	// Limit FPS
	static double lastTime = 0.0;
	static double time = Time::GetTime();
	static double deltaTime = time - lastTime;
	if (deltaTime >= MAX_PERIOD)
	{
		Renderer::Clear();

		// Draw skybox
		Skybox::Draw();

		// Draw objects
		s_defaultShader->Use();
		SceneManager::OnPaint(s_defaultShader);
		Camera::Paint(s_defaultShader);
		ObjectRenderPipeline::OnPaint(s_defaultShader);

		glFlush();
		glutPostRedisplay();
	}
}

void Application::End(void)
{
	s_activeSandbox->OnEnd();
	delete s_activeSandbox;
	delete s_defaultShader;
}

void Application::GetOpenGLInfo()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

void Application::InstallShader()
{
	s_defaultShader = new Shader(
		Resources::LoadTextFile("shaders/VertexShaderCode.glsl"),
		Resources::LoadTextFile("shaders/FragmentShaderCode.glsl"));
}
