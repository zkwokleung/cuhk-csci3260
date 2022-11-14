#pragma once
#include "../engine/Resources.h"
#include "../myapi.h"

#define PLAYER_MAX_TRAVEL_SPEED 10.f
#define PLAYER_TRANSLATION_ACCELERATION 0.5f
#define PLAYER_TRANSLATION_DECELERATION 0.1f
#define PLAYER_ROTATION_SPEED .05f

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

	float m_travelSpeed;
	bool m_moving, m_rolling;
	glm::vec3 m_velocity;

	static Player* s_activePlayer;
	static void cursor_position_callback(int x, int y);
	static void key_callback(unsigned char key, unsigned int action, int x, int y);
};