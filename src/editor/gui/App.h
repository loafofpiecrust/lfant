
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/ptr.h>

// External
#include <wx/app.h>

namespace lfant {
namespace editor {
namespace gui {

class Window;

class App : public wxApp
{
public:
	App();
	virtual ~App();

	virtual bool OnInit();
	virtual int MainLoop();

protected:
	ptr<Window> window;

private:
};

}
}
}