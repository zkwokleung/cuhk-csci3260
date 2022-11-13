#include "../myapi.h"
#include "../engine/Resources.h"
#include "player.h"
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
	Skybox* m_skybox;
	Player* m_player;
	ModelObject* m_rock;
};