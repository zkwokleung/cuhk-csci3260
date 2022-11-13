#include "../myapi.h"


class PlayerController : public Object
{
public:
	PlayerController();
	~PlayerController();

	void SetVelocity(glm::vec3 value);
	glm::vec3 GetVelocity() const;

	virtual void SetActive(bool active);
	virtual bool IsActive() const;

	virtual void OnPaint(Shader* shader);

	void SetCamera(Camera* camera);
	void SetBody(Transform* body);

private:
	static PlayerController* s_activeController;
	static void key_callback(unsigned char key, unsigned int action, int x, int y);
	static void cursor_pos_callback(int x, int y);

	float m_speed;
	float m_controlSpeed;
	glm::vec3 m_velocity;
	Camera* m_camera;
	Transform* m_body;
};

class FirstPersonPlayer : public Object
{
public:
	FirstPersonPlayer();
	~FirstPersonPlayer();

	virtual void SetActive(bool active);
	virtual bool IsActive() const;

private:
	Camera* m_camera;
	PlayerController* m_controller;
};