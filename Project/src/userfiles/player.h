#pragma once
#include "../engine/Resources.h"
#include "../myapi.h"

#define PLAYER_MAX_TRAVEL_SPEED 10.f
#define PLAYER_MAX_ROLLING_SPEED 0.1f
#define PLAYER_TRANSLATION_ACCELERATION 0.5f
#define PLAYER_TRANSLATION_DECELERATION 0.1f
#define PLAYER_ROLLING_ACCELERATION 0.5f
#define PLAYER_TRANSLATION_DECELERATION 0.1f
#define PLAYER_ROTATION_SPEED .05f

#define PLAYER_STATE_IDLE 0x0
#define PLAYER_STATE_FORWARD 0x1
#define PLAYER_STATE_BACKWARD 0x2
#define PLAYER_STATE_ROLLLEFT 0x3
#define PLAYER_STATE_ROLLRIGHT 0x4

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

	float m_travelSpeed, m_rollingSpeed;
	bool m_moving, m_rolling;
	glm::vec3 m_velocity;
	int m_translationState, m_rollingState;

	static Player* s_activePlayer;
	static void cursor_position_callback(int x, int y);
	static void key_callback(unsigned char key, unsigned int action, int x, int y);
};