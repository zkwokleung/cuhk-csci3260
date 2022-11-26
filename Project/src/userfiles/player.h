#pragma once
#include "../engine/Resources.h"
#include "../engine/Time.h"
#include "../myapi.h"
#include <glm/gtx/compatibility.hpp>
#include "laser.h"

#define PLAYER_MAX_TRAVEL_SPEED 1000.f
#define PLAYER_TRANSLATION_ACCELERATION 100.f
#define PLAYER_TRANSLATION_DECELERATION 50.f

#define PLAYER_MAX_ROLLING_SPEED 50.f
#define PLAYER_ROLLING_ACCELERATION 2.f
#define PLAYER_ROLLING_DECELERATION 1.f

#define PLAYER_SENSITIVITY 50.f
#define PLAYER_BACKWARD_SPEED .1f

#define PLAYER_STATE_IDLE 0x0
#define PLAYER_STATE_FORWARD 0x1
#define PLAYER_STATE_BACKWARD 0x2
#define PLAYER_STATE_MOVELEFT 0x3
#define PLAYER_STATE_MOVERIGHT 0x4

#define PLAYER_STATE_ROLLLEFT 0x3
#define PLAYER_STATE_ROLLRIGHT 0x4

class Player : public Object, private IKeyCallback, private ICursorPosCallback, private ICollisionCallback, private IMouseButtonCallback
{
public:
	Player(void);
	~Player();

	virtual void OnEnable(void);
	virtual void OnDisable(void);
	virtual void OnUpdate(void);
	virtual void OnPaint(Shader* shader);
	virtual void OnCollision(Collider* self, Collider* other);

private:
	PerspectiveCamera* m_camera;
	ModelObject* m_model;
	PointLight* m_light;

	void cursor_position_callback(int x, int y);
	void key_callback(unsigned char key, unsigned int action, int x, int y);
	virtual void mouse_button_callbak(int button, int state, int x, int y);

	float m_travelSpeed, m_horizontalSpeed, m_rollingSpeed;
	glm::vec3 m_velocity;
	int m_translationState, m_horizontalState, m_rollingState;
};