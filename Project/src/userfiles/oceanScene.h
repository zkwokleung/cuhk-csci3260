#pragma once
#include "../myapi.h"
#include "player.h"

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

	void key_callback(unsigned char key, unsigned int action, int x, int y);
};