/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2013-02-13 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/
#pragma once

// External

// Internal
#include "Subsystem.hpp"

namespace CEGUI
{
class OpenGL3Renderer;
class Window;
class WindowManager;
class System;
class GUIContext;
}

namespace sfs
{

class UserInterface : public Subsystem
{
public:
	UserInterface();
	virtual ~UserInterface();

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

	virtual void CreateWindow(string layout);

	virtual void OnKey(uint16_t key, bool pressed);
	virtual void OnChar(char key);
	virtual void OnMouseButton(uint16_t btn, bool pressed);
	virtual void OnMouseMove(float x, float y);
	virtual void OnWindowResize(uint width, uint height);

protected:
	CEGUI::OpenGL3Renderer* renderer;
	CEGUI::Window* rootWindow;
	CEGUI::WindowManager* windowManager;
	vector<CEGUI::Window*> windows;
	CEGUI::System* system;
	CEGUI::GUIContext* context;

	bool resized = false;
	uvec2 size;
};

}
