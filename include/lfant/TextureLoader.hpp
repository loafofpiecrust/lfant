/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2013-01-13 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/
#pragma once

#include <lfant/stdafx.hpp>

// External

// Internal
#include <lfant/Texture.hpp>

namespace lfant
{
	Texture LoadTexture( string name, int buffer = 0 );

	Texture LoadBMP( string name, int buffer = 0 );
	Texture LoadJPEG( string name, int buffer = 0 );
	Texture LoadPNG( string name, int buffer = 0 );
	Texture LoadTGA( string name, int buffer = 0 );
}