#include "Application.h"

// screen setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

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
	get_OpenGL_info();

	srand(time(NULL));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void Application::MainLoop(void)
{
}

void Application::End(void)
{
}

void Application::get_OpenGL_info()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}