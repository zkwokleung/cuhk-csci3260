#include "../engine/Random.h"
#include "../engine/Resources.h"
#include "../myapi.h"
#include "player.h"
#include <string>

#define PLANET_INITIAL_POSITION glm::vec3(1000.0f, .0f, -19000.f)

#define SPACE_ROCK_NUM 200
#define SPACE_ROCK_MIN_DISTANCE 5.f
#define SPACE_ROCK_MAX_DISTANCE 10.f
#define SPACE_ROCK_MIN_SCALE 0.1f
#define SPACE_ROCK_MAX_SCALE .2f

#define SPACE_CRAFT_NUM 20
#define SPACE_CRAFT_MIN_SCALE 10.f
#define SPACE_CRAFT_MAX_SCALE 30.f
#define SPACE_CRAFT_MIN_POSITION_X -2000.f
#define SPACE_CRAFT_MAX_POSITION_X 2000.f
#define SPACE_CRAFT_MIN_POSITION_Y -1000.f
#define SPACE_CRAFT_MAX_POSITION_Y 1000.f
#define SPACE_CRAFT_MIN_POSITION_Z -10000.f
#define SPACE_CRAFT_MAX_POSITION_Z 100.f
#define SPACE_CRAFT_TELEPORT_INTERVAL 5.f
#define SPACE_CRAFT_SELF_ROTATE_SPEED 100.f

class SpaceScene : public Scene
{
public:
	SpaceScene();
	~SpaceScene();

	virtual void OnInitialize();
	virtual void OnUpdate(void);
	virtual void OnPaint(Shader* shader);
	virtual void OnEnd();

private:
	Skybox* m_skybox;
	Player* m_player;
	Object* m_rocksContainer;
	ModelObject* m_planet, * m_rocks[SPACE_ROCK_NUM], * m_spaceCrafts[SPACE_CRAFT_NUM];
	PointLight* m_planetLight, * m_craftLights[SPACE_CRAFT_NUM];

	static void key_callback(unsigned char key, unsigned int action, int x, int y);
};