#include "Application.h"
#include "Resources.h"

// screen setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

Shader* Application::s_defaultShader = nullptr;

void Application::Initialize(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Project");
	glutDisplayFunc(MainLoop);
}

void Application::Run(void)
{
	Setup();
	glutMainLoop();
	End();
}

void Application::Setup(void)
{
	// Initialize random seed
	srand(time(NULL));

	// Setup OpenGL
	glewInit();
	GetOpenGLInfo();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Initialize fields and API
	Input::Init();
	Skybox::Init();
	InstallShader();
	std::vector<std::string> faces = {
		"skybox/right.bmp",
		"skybox/left.bmp",
		"skybox/top.bmp",
		"skybox/bottom.bmp",
		"skybox/front.bmp",
		"skybox/back.bmp"
	};

}

void Application::MainLoop(void)
{
	// Draw skybox
	Skybox::Draw();

	// Draw objects
	s_defaultShader->Use();
	Camera::OnPaint(s_defaultShader);
	ObjectRenderPipeline::OnPaint(s_defaultShader);
	SceneManager::OnPaint(s_defaultShader);
}

void Application::End(void)
{
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
	s_defaultShader = new Shader(Resources::LoadTextFile("shaders/VertexShaderCode.glsl"),
		Resources::LoadTextFile("shaders/FragmentShaderCode.glsl"));
}
