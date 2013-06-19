
#include <editor/Editor.h>

// External
#include <wx/app.h>

// Internal
#include <lfant/Scene.h>
#include <lfant/Time.h>
#include <lfant/Renderer.h>
#include <lfant/Input.h>
#include <lfant/Physics.h>
#include <lfant/ScriptSystem.h>
#include <lfant/Console.h>
#include <lfant/SystemInfo.h>
#include <lfant/FileSystem.h>

#include <editor/gui/App.h>

namespace lfant
{
namespace editor
{

void Launch()
{
	printf("YOOO");
	game = new Editor();
	game->standAlone = false;
	game->Init();
	game->Update();
	delete game;
}

Editor::Editor()
{
}

Editor::~Editor()
{
}

void Editor::Init()
{
//	LoadFile("settings/game.cfg");


	console->Init();
	Log("Creating gui app...");

	app = new gui::App;
	wxApp::SetInstance(app);
	int i = 0;
	wxEntryStart(i, (char**)0);
	app->OnInit();
	app->OnRun();
//	app->OnExit();
	
	Log("Loading other stuff.");

	fileSystem->Init();
	systemInfo->Init();
	time->Init();
	physics->Init();
//	renderer->Init();
	scene->Init();
	input->Init();
//	audio->Init();
//	network->Init();
//	scriptSystem->Init();

	/// @todo Init renderer as Editor, eg. without the window opening process
	///			split 'window' into it's own subsystem?
}

void Editor::Update()
{
	while(!IsExited())
	{

		if(playing)
		{
			time->Update();
			physics->Update();
			scene->Update();
			input->Update();
		//	userInterface->Update();
		//	renderer->Update();
		//	network->Update();
		}
		// Call Editor subsystems
		// eg. ?
	}
	Destroy();
}

void Editor::OnDestroy()
{
	Game::OnDestroy();
}

}
}
