#include "../engine/Random.h"
#include "../engine/Resources.h"
#include "../myapi.h"
#include "firstPersonPlayer.h"
#include "player.h"
#include <string>

#define PLANET_INITIAL_POSITION glm::vec3(1000.0f, .0f, -19000.f)

#define SPACE_ROCK_NUM 200
#define SPACE_ROCK_MIN_DISTANCE 5.f
#define SPACE_ROCK_MAX_DISTANCE 10.f
#define SPACE_ROCK_MIN_SCALE 0.1f
#define SPACE_ROCK_MAX_SCALE .2f

#define SPACE_CRAFT_NUM 10
#define SPACE_CRAFT_MIN_SCALE 0.1f
#define SPACE_CRAFT_MAX_SCALE 20.f

class SpaceScene : public Scene
{
  public:
    SpaceScene();
    ~SpaceScene();

    virtual void OnInitialize();
    virtual void OnPaint(Shader *shader);
    virtual void OnEnd();

  private:
    Skybox *m_skybox;
    Player *m_player;
    ModelObject *m_planet;
    Object *m_rocksContainer;
    ModelObject *m_rocks[SPACE_ROCK_NUM];
    ModelObject *m_spaceCrafts[SPACE_CRAFT_NUM];
    PointLight *m_planetLight;
};