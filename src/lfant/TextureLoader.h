/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once

#include <lfant/stdafx.h>

// External

// Internal
#include <lfant/Texture.h>

namespace lfant
{
Texture LoadTexture( string name, int buffer = 0 );

Texture LoadBMP( string name, int buffer = 0 );
Texture LoadJPEG( string name, int buffer = 0 );
Texture LoadPNG( string name, int buffer = 0 );
Texture LoadTGA( string name, int buffer = 0 );
}
