/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-17 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Renderer.h>

// Internal

// External

namespace lfant {

/// @todo Android interface usage

void Renderer::SetWindowTitle(string title)
{
	windowTitle = title;
}

void Renderer::SetResolution(ivec2 res)
{
	// Possible?
}

void Renderer::SetVersion(byte major, byte minor)
{
	// Not used
}

void Renderer::SetWindowPos(ivec2 pos)
{
	// Not Possible
}

void Renderer::HideMouse(bool hide)
{
	// No mouse cursor on mobile
}

}