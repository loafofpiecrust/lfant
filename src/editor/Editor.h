
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Game.h>

// External
#include <forward_list>

namespace lfant {
namespace editor {

namespace gui {
class App;
}

/**
 *
 */
class Editor : public lfant::Game
{
public:
	Editor();
	virtual ~Editor();

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	void LoadProject(string dir);


protected:
	ptr<gui::App> app;
	bool playing = false;

private:

};

extern "C" void Launch();

}
}
