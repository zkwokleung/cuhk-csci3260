#pragma once
#include "glew/glew.h"
#include "GLFW/glfw3.h"

class ShadowMapper {
public:
	ShadowMapper();
	~ShadowMapper();

	void Bind();
	void Unbind();

private:
	unsigned int m_depthMapFBO;
	unsigned int m_depthMap;

	static const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
};