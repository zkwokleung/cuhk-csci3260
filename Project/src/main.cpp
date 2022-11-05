/*
Student Information
Student ID: 1155149068,
Student Name: SZE-TO Kwok Leung,
*/

#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "myapi.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <sstream>

// screen setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void get_OpenGL_info()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

void sendDataToOpenGL()
{

}

void initializedGL(void) //run only once
{
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not OK." << std::endl;
	}

	get_OpenGL_info();
	sendDataToOpenGL();

	srand(time(NULL));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void paintGL(void)  // Main Loop
{
	glClearColor(1, 1, 1, 1);  // (In fact, this is the default.)
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0); // red
	glVertex2f(-0.8, -0.8);
	glColor3f(0, 1, 0); // green
	glVertex2f(0.8, -0.8);
	glColor3f(0, 0, 1); // blue
	glVertex2f(0, 0.9);
	glEnd();


	glutSwapBuffers(); // Required to copy color buffer onto the screen.
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void onEnd(void)
{

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);         // Size of display area, in pixels.
	glutInitWindowPosition(100, 100);     // Location of window in screen coordinates.
	glutCreateWindow("GL RGB Triangle"); // Parameter is window title.
	glutDisplayFunc(paintGL);            // Called when the window needs to be redrawn.

	initializedGL();

	glutMainLoop(); // Run the event loop!  This function does not return.

	onEnd(); // Clean up the program

	return 0;
}