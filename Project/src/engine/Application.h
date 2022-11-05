#pragma once

#include "../myapi.h"
#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <freeglut/freeglut_std.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>

class Application
{
public:
	static void Initialize(int argc, char* argv[]);
	static void Run(void);

private:
	static void Setup(void);
	static void MainLoop(void);
	static void End(void);

	static void get_OpenGL_info();
};