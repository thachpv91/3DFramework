#include "Defines.h"
#include "Game.h"
#include "HelloTriangles.h"

#ifdef USE_MEMDGB
#include <vld.h>
#endif

int main()
{
	Game* game = Game::GetInstance();

	game->Init("../Resources/", WIDTH, HEIGHT);

	game->SetState(new HelloTriangles());

	while ( game->IsRuning() )
	{
		game->Update();
	}
	Game::FreeInstance();

	LOGI("\n");
}