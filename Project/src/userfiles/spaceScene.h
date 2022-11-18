#include "../myapi.h"
#include "../engine/Resources.h"
#include "../engine/Random.h"
#include "player.h"
#include "firstPersonPlayer.h"
#include <string>

#define SPACE_ROCK_NUM 200

class SpaceScene : public Scene
{
public:
	SpaceScene();
	~SpaceScene();

	virtual void OnInitialize();
	virtual void OnPaint(Shader* shader);
	virtual void OnEnd();

private:
	Skybox* m_skybox;
	Player* m_player;
	ModelObject* m_planet;
	Object* m_rocksContainer;
	ModelObject* m_rocks[SPACE_ROCK_NUM];
	PointLight* m_planetLight;
};