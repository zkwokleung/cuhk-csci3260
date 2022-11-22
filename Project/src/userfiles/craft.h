#pragma once
#include "../myapi.h"
#include "../engine/Resources.h"
#include "../engine/Random.h"
#include "../engine/Time.h"

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

class Craft : public Object
{
public:
	Craft();
	~Craft();

	virtual void SetActive(bool active);

	virtual void OnEnable(void);
	virtual void OnUpdate(void);

private:
	ModelObject* m_model;
	PointLight* m_light;

	static Mesh* s_mesh;
	static Texture* s_texture;
};