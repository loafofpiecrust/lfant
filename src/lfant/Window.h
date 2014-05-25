
#include "lfant/Subsystem.h"


struct GLFWwindow;

namespace sf {
class Window;
class RenderWindow;
}

namespace lfant {
	
namespace editor {
namespace gui {
	class GLCanvas; 
}
}
	
class Window : public Subsystem
{
	friend class editor::gui::GLCanvas;
public:
	Window(Game* game);
	virtual ~Window();

	
	virtual void Serialize(Properties* prop);

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Deinit();

	void SetPosition(ivec2 pos);
	ivec2 GetPosition() const;

	void Resize(ivec2 size);
	ivec2 GetSize() const;

	void HideCursor(bool hide);

	void SetCursorPos(ivec2 pos);
	ivec2 GetCursorPos() const;

	void SetTitle(string title);

//	sf::RenderWindow* GetHandle();
	GLFWwindow* GetHandle();

	void Open(ivec2 res);
	void Close();

	bool IsOpen();
	
	bool SetVsync(int enable);
	
	// Multi-window functions. Currently only partially supported
	void SetCurrent();

	int fsaa = 2;
	Range<int> version {4,2};
	bool fullscreen = false;
	bool resizable = false;

private:
	static void OnFrameResize(GLFWwindow* window, int width, int height);
	static void OnWindowResize(GLFWwindow* window, int width, int height);
	static void OnWindowMove(GLFWwindow* window, int x, int y);
	static void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
	static void OnCursorMove(GLFWwindow* window, double x, double y);
	static void OnCursorEnter(GLFWwindow* window, int entered);
	static void OnScroll(GLFWwindow* window, double xoff, double yoff);
	static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void OnChar(GLFWwindow* window, uint letter);

	ptr<sf::RenderWindow> window;
	GLFWwindow* handle = nullptr;

	ivec2 resolution {800,600};
	ivec2 position {0,0};
	ivec2 cursorPos {0,0};
	string title = "";
	bool needsInit = true;
};

}
