
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/ptr.h>

// external
#include <wx/frame.h>
#include <wx/glcanvas.h>

namespace lfant {
namespace editor {
namespace gui
{

class GLCanvas : public wxGLCanvas
{
public:
	GLCanvas(wxFrame* parent, int id);

	void Init();

	void Paint();
protected:
	void Render(wxDC& dc);

	void OnPaint(wxPaintEvent& evt);
	void OnResize(wxSizeEvent& evt);
	void OnMouseMove(wxMouseEvent& evt);
	void OnMouseButton(wxMouseEvent& evt);
	void OnKeyPress(wxKeyEvent& evt);
	void OnKeyRelease(wxKeyEvent& evt);
	void OnChar(wxKeyEvent& evt);

	void OnKey(wxKeyEvent& evt, int action);

	ptr<wxGLContext> context = nullptr;
	bool inited = false;
private:
};

}
}
}