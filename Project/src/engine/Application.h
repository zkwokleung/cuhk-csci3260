#pragma once

#include <glew/glew.h>
#include <freeglut/glut.h>
#include <freeglut/freeglut.h>
#include <freeglut/freeglut_std.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include "../myapi.h"

class Application
{
public:
	// Initialize the application environment
	static void Initialize(int argc, char* argv[]);
	// Start running the application
	static void Run(void);

private:
	static Sandbox* s_activeSandbox;
	static Shader* s_defaultShader;

	// Set up the application to ready to use
	static void Setup(void);
	// Main loop is called every loop
	static void MainLoop(void);
	// End is called when is main loop is done
	static void End(void);
	// Get the information that is visible to OpenGL about this PC
	static void GetOpenGLInfo();
	// Install the default shader
	static void InstallShader();
};