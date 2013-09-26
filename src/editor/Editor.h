
#pragma once
#include <lfant/stdafx.h>

// External
#include <forward_list>

// Internal
#include <lfant/Game.h>


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


protected:
	ptr<gui::App> app;
	bool playing = false;

private:

};

extern "C" void Launch();

}
}
