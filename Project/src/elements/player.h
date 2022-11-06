#pragma once
#include "../engine/Resources.h"
#include "../myapi.h"

class Player : public Object {
public:
	Player(void);
	~Player();

	virtual void OnPaint(Shader* shader);

private:
	Camera* m_camera;
	ModelObject* m_model;
	PointLight* m_light;
};