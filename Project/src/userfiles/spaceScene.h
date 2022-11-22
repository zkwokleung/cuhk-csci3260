#include "../engine/Random.h"
#include "../engine/Resources.h"
#include "../myapi.h"
#include "player.h"
#include "craft.h"
#include <string>

#define PLANET_INITIAL_POSITION glm::vec3(1000.0f, .0f, -19000.f)

#define SPACE_ROCK_NUM 200
#define SPACE_ROCK_MIN_DISTANCE 5.f
#define SPACE_ROCK_MAX_DISTANCE 10.f
#define SPACE_ROCK_MIN_SCALE 0.1f
#define SPACE_ROCK_MAX_SCALE .2f

#define SPACE_CRAFT_NUM 20

class SpaceScene : public Scene, private IKeyCallback
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
	ModelObject* m_planet, * m_rocks[SPACE_ROCK_NUM];
	Craft* m_spaceCrafts[SPACE_CRAFT_NUM];
	PointLight* m_planetLight;

	void key_callback(unsigned char key, unsigned int action, int x, int y);
};