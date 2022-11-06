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
	static void Initialize(int argc, char* argv[]);
	static void Run(void);

private:
	static Shader* s_defaultShader;
	static void Setup(void);
	static void MainLoop(void);
	static void End(void);

	static void GetOpenGLInfo();
	static void InstallShader();
};