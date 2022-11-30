#pragma once
#include "../myapi.h"
#include "player.h"
#include "mountain.h"

#define MOUNTAIN_COUNT 11
#define MOUNTAIN_SIZE_MIN 1000
#define MOUNTAIN_SIZE_MAX 10000
#define MOUNTAIN_POS_X_MIN -100000
#define MOUNTAIN_POS_X_MAX 1000000
#define MOUNTAIN_POS_Z_MIN -100000
#define MOUNTAIN_POS_Z_MAX 1000000

class OceanScene : public Scene, private IKeyCallback
{
public:
	OceanScene();
	~OceanScene();

	virtual void OnInitialize();
	virtual void OnUpdate(void);
	virtual void OnPaint(Shader* shader);
	virtual void OnEnd();

private:
	Skybox* m_skybox;
	Player* m_player;
	ModelObject* m_ground;
	Mountain* m_mountains[MOUNTAIN_COUNT];

	DirectionalLight* m_dirLight;

	void key_callback(unsigned char key, unsigned int action, int x, int y);
};