
#include <galaga/Galaga.h>
#include <lfant/Console.h>

using namespace lfant;

int main()
{
	printf("Launching\n");
	game = new galaga::Galaga();
	game->standAlone = true;
	printf("Initing\n");
	game->Init();
	Log("Game initialised");
	game->Update();
	Log("Game ending");
	delete game;
}