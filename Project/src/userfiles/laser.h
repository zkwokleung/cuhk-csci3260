#pragma once
#include "../myapi.h"
#include "../engine/Resources.h"
#include "../engine/Random.h"
#include "../engine/Time.h"

#define LASER_LIFE 3.f
#define LASER_SPEED 1250.f

class Laser : public Object
{
public:
	Laser();
	~Laser();

	virtual void SetActive(bool active);

	virtual void OnEnable(void);
	virtual void OnUpdate(void);

private:
	ModelObject* m_model;
	PointLight* m_light;

	static Model* s_mesh;
	static Texture* s_texture;
};