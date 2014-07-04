module lfant.window;

import lfant.game;
import lfant.transform;
import lfant.properties;

import derelict.sfml2.system;
import derelict.sfml2.window;
import derelict.sfml2.graphics;
import derelict.opengl3.gl3;

import std.stdio;

class Window : Subsystem
{
	this(Game g)
	{
		super(g);
	}
	
	~this()
	{
		writeln("window");
	}
	
	override void serialize(Properties prop)
	{
		prop.value("title", title);
		prop.value("fsaa", fsaa);
	//	prop.value("version", glVersion);
	
		if(prop.mode == Properties.Mode.Input)
		{
			vec2i res, pos;
		//	prop.value("resolution", res);
		//	prop.value("position", pos);
			if(!opened)
			{
				open(res, pos);
			}
			else
			{
				size = res;
				position = pos;
			}
		}
	}
	
	override void initialize()
	{
		DerelictGL3.load();
		DerelictSFML2System.load();
		DerelictSFML2Window.load();
		DerelictSFML2Graphics.load();
		
		loadFile("settings/window.prop");
		
		if(!opened)
		{
			open(vec2i(889, 500));
		}
	}
	
	override void update(in float delta)
	{
		sfEvent evt;
		while(sfRenderWindow_pollEvent(handle, &evt))
		{
			switch(evt.type)
			{
				case sfEvtMouseMoved:
				{
					game.input.onMouseMove(vec2i(evt.mouseMove.x, evt.mouseMove.y));
					break;
				}
				case sfEvtKeyPressed:
				{
					game.input.onKey(evt.key.code, 1, 0);
					break;
				}
				case sfEvtKeyReleased:
				{
					game.input.onKey(evt.key.code, 0, 0);
					break;
				}
				case sfEvtMouseButtonPressed:
				{
					game.input.onMouseButton(evt.mouseButton.button, 1, 0);
					break;
				}
				case sfEvtMouseButtonReleased:
				{
					game.input.onMouseButton(evt.mouseButton.button, 0, 0);
					break;
				}
				case sfEvtResized:
				{
					game.renderer.resize(vec2i(evt.size.width, evt.size.height));
					break;
				}
				case sfEvtClosed:
				{
					close();
					game.exit();
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
	
	override void render()
	{
		sfRenderWindow_display(handle);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	override void deinit()
	{
		if(handle)
		{
			close();
			sfRenderWindow_destroy(handle);
			handle = null;
		}
		super.deinit();
	}
	
	void open(vec2i size, vec2i pos = vec2i(0))
	{
		if(!game.standAlone) return;
		game.log("Opening window...");
		uint style = sfTitlebar|sfClose;
		if(fullscreen)
		{
			style |= sfFullscreen;
		}
		auto settings = sfContextSettings(32,8,fsaa, glVersion.x, glVersion.y);
		handle = sfRenderWindow_create(sfVideoMode(size.x, size.y), "", style, &settings);
	
		if(!sfRenderWindow_isOpen(handle))
		{
			game.log("Window failed to open");
			return;
		}
	
		sfRenderWindow_setKeyRepeatEnabled(handle, false);
		sfRenderWindow_setActive(handle, true);
		sfRenderWindow_setFramerateLimit(handle, 100);
		
		DerelictGL3.reload();
	}
	
	void close()
	{
		if(handle) sfRenderWindow_close(handle);
	}
	
	void setCurrent()
	{
		sfRenderWindow_setActive(handle, true);
	}
	
	@property
	{
		vec2i position()
		{
			return cast(vec2i)sfRenderWindow_getPosition(handle);
		}
		void position(vec2i pos)
		{
			sfRenderWindow_setPosition(handle, sfVector2i(pos.x, pos.y));
		}

		vec2i size()
		{
			return cast(vec2i)sfRenderWindow_getSize(handle);
		}
		void size(vec2i s)
		{
			sfRenderWindow_setSize(handle, sfVector2u(s.x, s.y));
		}

		vec2i cursorPos()
		{
			return cast(vec2i)sfMouse_getPositionRenderWindow(handle);
		}
		void cursorPos(vec2i pos)
		{
			sfMouse_setPositionRenderWindow(sfVector2i(pos.x, pos.y), handle);
		}

		void vsync(bool enable)
		{
			sfRenderWindow_setVerticalSyncEnabled(handle, enable);
		}

		void title(string t)
		{
			sfRenderWindow_setTitle(handle, t.ptr);
			_title = t;
		}
		private ref string title()
		{
			return _title;
		}

		bool opened() const
		{
			if(!handle) return false;
			else return cast(bool)sfRenderWindow_isOpen(handle);
		}
	}
	
private:
	
	static bool needsInit = true;

	sfRenderWindow* handle = null;

	vec2i glVersion = vec2i(4, 2);
	string _title;
	int fsaa = 2;
	bool fullscreen = false;
	bool resizable = false;
}