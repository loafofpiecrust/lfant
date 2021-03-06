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

// Internal
#include <lfant/Texture.h>
#include <lfant/Shader.h>
#include <lfant/Object.h>

// External

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Rendering
 *	 @{
 */

/**
 *
 */
class Material : public Object
{
public:
	Material();

	virtual void Serialize(Properties *prop);

	// Path and name for the texture file.
	std::shared_ptr<Texture> texture;
//	uint32 textureUnif;
	std::shared_ptr<Shader> shader;
	vec2 tiling { 1, 1 };
	vec2 offset { 0, 0 };
	u8vec4 color { 255, 255, 255, 255 };
//	bool loaded = false;
};

/// @}
/// @}
}
