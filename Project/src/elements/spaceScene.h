#include "../myapi.h"
#include "../engine/Resources.h"
#include "player.h"

class SpaceScene : public Scene
{
public:
	SpaceScene();
	~SpaceScene();

private:
	Player* m_player;
};