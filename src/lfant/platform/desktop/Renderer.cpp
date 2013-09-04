/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-17 by Taylor Snead
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

#include <lfant/Renderer.h>

// Internal
#include <lfant/Game.h>

// External
#include <GLFW/glfw3.h>

namespace lfant {

void Renderer::SetWindowTitle(string title)
{
	windowTitle = title;
	glfwSetWindowTitle(window, title.c_str());
}

void Renderer::SetResolution(ivec2 res)
{
	resolution = res;
	glfwSetWindowSize(window, res.x, res.y);
}

void Renderer::SetVersion(byte major, byte minor)
{
	version = {major, minor};
	if(!OpenWindow())
	{
		game->Exit();
	}
}

void Renderer::SetWindowPos(ivec2 pos)
{
	windowPos = pos;
	glfwSetWindowPos(window, pos.x, pos.y);
}

void Renderer::HideMouse(bool hide)
{
	glfwSetInputMode(window, GLFW_CURSOR, !hide);
}

}