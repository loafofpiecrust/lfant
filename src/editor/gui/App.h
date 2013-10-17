
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/ptr.h>
#include <editor/gui/Window.h>

// External
#include <wx/app.h>

namespace lfant {
namespace editor {
namespace gui {

class App : public wxApp
{
public:
	App();
	virtual ~App();

	virtual bool OnInit();
//	virtual int MainLoop();

	void ActivateRenderLoop(bool on);
	void OnIdle(wxIdleEvent& evt);

protected:
	ptr<Window> window = nullptr;
	bool render_loop_on = false;

private:
};

DECLARE_APP(lfant::editor::gui::App)

}
}
}