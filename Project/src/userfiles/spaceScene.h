#include "../myapi.h"
#include "../engine/Resources.h"
#include "player.h"
#include "firstPersonPlayer.h"
#include <string>

class SpaceScene : public Scene
{
public:
	SpaceScene();
	~SpaceScene();

	virtual void OnInitialize();
	virtual void OnPaint(Shader* shader);
	virtual void OnEnd();

private:
	Camera* m_cam;
	Skybox* m_skybox;
	Player* m_player;
	FirstPersonPlayer* m_fpp;
	ModelObject* m_rock;
	static void cursor_position_callback(int x, int y);
};