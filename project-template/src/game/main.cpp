
#include <lfant/stdafx.h>

#include <projectName/projectTitle.h>

using namespace projectName;

int main(int argc, const char* argv[])
{
	projectTitle* game = new projectTitle();
	game->standAlone = true;
	game->Init();
	while(!game->IsExited())
	{
		game->Update();
	}
	game->Destroy();
}
