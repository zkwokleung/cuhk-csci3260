#pragma once
#include "../engine/Resources.h"
#include "../myapi.h"

class Player : public Object {
public:
	Player(void);
	~Player();

	virtual bool IsActive() const;
	virtual void SetActive(bool active);
	virtual void OnPaint(Shader* shader);

private:
	Camera* m_camera;
	ModelObject* m_model;
	PointLight* m_light;

	float m_rotationAcceleration, m_translationAcceleration;

	static Player* s_activePlayer;
	static void cursor_position_callback(int x, int y);
};