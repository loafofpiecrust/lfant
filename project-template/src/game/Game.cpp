
#include <projectName/projectTitle.h>

// External

// Internal
#include <lfant/Renderer.h>
#include <lfant/Time.h>
#include <lfant/Console.h>
#include <lfant/SystemInfo.h>
#include <lfant/Physics.h>
#include <lfant/Scene.h>
#include <lfant/Entity.h>
#include <lfant/Input.h>

using namespace lfant;

namespace projectName {

extern "C" void Launch()
{
	// Called from the launcher
	game = new projectTitle();
	game->standAlone = true;
	game->Init();
	game->Update();
	delete game;
}

projectTitle::projectTitle()
{
}

projectTitle::~projectTitle()
{
}

void projectTitle::Init()
{
	Game::Init();

	// Do some stuff.
}

void projectTitle::Update()
{
	// Allows for custom loop setups
	while(!IsExited())
	{
		Game::Update();
		// Add more systems here.
	}
	Game::Destroy();
	// Code here is equivalent to OnDestroy()
}

void projectTitle::OnDestroy()
{
	// Called at the end of Destroy()
}

}