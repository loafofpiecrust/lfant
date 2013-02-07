
#include "projectTitle.hpp"

// External

// Internal
#include "Renderer.hpp"
#include "Time.hpp"
#include "Console.hpp"
#include "SystemInfo.hpp"
#include "Physics.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Input.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"
#include "Type.hpp"

#include "TextureLoader.hpp"
#include "ShaderLoader.hpp"

using namespace sfs;

extern "C" void Launch()
{
	game = new projectTitle();
	game->standAlone = true;
	game->Init();
	game->destroy = false;
	Log("Game initialised");
	while (!game->destroy)
	{
		game->Update();
	}
	Log("About to destroy game");
	game->Destroy();
}

void projectTitle::Init()
{
	console = new Console;
	console->Init();

	Log("ShadowFox Engine launched.");

	systemInfo = new SystemInfo;
	time = new Time;
	physics = new Physics;
	scene = new Scene;
	renderer = new Renderer;
	input = new Input;

	renderer->SetVersion(3, 3);

	Log("Subsystems instantiated.");

	systemInfo->Init();
	time->Init();
	//physics->Init();
	renderer->Init();
	scene->Init();
	input->Init();

	Log("Subsystems initialized");
}

void projectTitle::Update()
{
	renderer->PreUpdate();
	time->Update();
	//physics->Update();
	scene->Update();
	input->Update();
	renderer->Update();
}
