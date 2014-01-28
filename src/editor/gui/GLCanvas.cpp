
#include <editor/gui/GLCanvas.h>

// internal
#include <lfant/Game.h>
#include <lfant/Scene.h>
#include <lfant/Renderer.h>
#include <lfant/Console.h>
#include <lfant/UserInterface.h>
#include <lfant/Input.h>
#include <lfant/util/lexical_cast.h>

// external
#include <wx/dcclient.h>

namespace lfant {
namespace editor {
namespace gui {

int shizargs[] = {
	WX_GL_RGBA,
	WX_GL_DOUBLEBUFFER,
	WX_GL_DEPTH_SIZE, 16,
	0, 0
};

GLCanvas::GLCanvas(wxFrame* parent, int id) :
	wxGLCanvas(parent, id, shizargs, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE, "GLCanvas")
{
	context = new wxGLContext(this);
	Connect(id, wxEVT_PAINT, (wxObjectEventFunction)&GLCanvas::OnPaint);
	Connect(id, wxEVT_SIZE, (wxObjectEventFunction)&GLCanvas::OnResize);
	Connect(id, wxEVT_MOTION, (wxObjectEventFunction)&GLCanvas::OnMouseMove);

	Connect(id, wxEVT_KEY_DOWN, (wxObjectEventFunction)&GLCanvas::OnKeyPress);
	Connect(id, wxEVT_KEY_UP, (wxObjectEventFunction)&GLCanvas::OnKeyRelease);
	Connect(id, wxEVT_CHAR, (wxObjectEventFunction)&GLCanvas::OnChar);

	// Mouse Clicks
	Connect(id, wxEVT_LEFT_DOWN, (wxObjectEventFunction)&GLCanvas::OnMouseButton);
	Connect(id, wxEVT_LEFT_UP, (wxObjectEventFunction)&GLCanvas::OnMouseButton);
	Connect(id, wxEVT_MIDDLE_DOWN, (wxObjectEventFunction)&GLCanvas::OnMouseButton);
	Connect(id, wxEVT_MIDDLE_UP, (wxObjectEventFunction)&GLCanvas::OnMouseButton);
	Connect(id, wxEVT_RIGHT_DOWN, (wxObjectEventFunction)&GLCanvas::OnMouseButton);
	Connect(id, wxEVT_RIGHT_UP, (wxObjectEventFunction)&GLCanvas::OnMouseButton);
}

void GLCanvas::Init()
{
/*	SetCurrent(*context);
	wxClientDC dc(this);
	game->renderer->Init();
	game->userInterface->Init();*/
}

void GLCanvas::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void GLCanvas::Paint()
{
//	wxClientDC dc(this);
	wxPaintDC dc(this);
	Render(dc);
}

void GLCanvas::Render(wxDC& dc)
{
//	Log("Rendering the shizz");
	if(!IsShown()) return;

	SetCurrent(*context);

	if(!inited)
	{
		inited = true;
		game->Init();
	}

	game->scene->Render();
	game->renderer->Update();
	game->userInterface->Update();
	SwapBuffers();
	game->renderer->PreUpdate();

}

void GLCanvas::OnResize(wxSizeEvent& evt)
{
	Refresh();
	game->renderer->OnSetResolution(nullptr, GetSize().x, GetSize().y);
}

void GLCanvas::OnKeyPress(wxKeyEvent& evt)
{
	OnKey(evt, 1);
}

void GLCanvas::OnKeyRelease(wxKeyEvent& evt)
{
	OnKey(evt, 0);
}

void GLCanvas::OnKey(wxKeyEvent& evt, int action)
{
	int key = evt.GetKeyCode();
	switch(evt.GetKeyCode())
	{
		case WXK_UP: key = Key["up"]; break;
		case WXK_DOWN: key = Key["down"]; break;
		case WXK_RIGHT: key = Key["right"]; break;
		case WXK_LEFT: key = Key["left"]; break;
	}

	game->input->OnKeyPress(key, evt.GetRawKeyCode(), action, evt.GetModifiers());
//	Log("Key action(", action, "): ", key);

	if(action == 1)
	{
		evt.Skip();
	}
}

void GLCanvas::OnChar(wxKeyEvent& evt)
{
	game->input->OnCharPress(evt.GetUnicodeKey());
}

void GLCanvas::OnMouseMove(wxMouseEvent& evt)
{
	game->input->SetMousePos(ivec2(evt.GetX(), evt.GetY()));
	game->input->OnMouseMove(vec2(evt.GetX(), evt.GetY()));
}

void GLCanvas::OnMouseButton(wxMouseEvent& evt)
{
//	Log("Click occurred at (physical)", lexical_cast<string>(vec2(evt.GetX(), evt.GetY())));
	int btn = 0;
	switch(evt.GetButton())
	{
		case wxMOUSE_BTN_LEFT: btn = Key["MouseLeft"]; break;
		case wxMOUSE_BTN_RIGHT: btn = Key["MouseRight"]; break;
		case wxMOUSE_BTN_MIDDLE: btn = Key["MouseMiddle"]; break;
	}
	btn -= 400;

	game->input->OnMouseButton(btn, evt.ButtonDown() ? 1 : 0, 0);
	if(evt.LeftDown())
	{
		evt.Skip();
	}
}

}
}
}