/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2013-03-18 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Material.h>

// Internal
#include <lfant/Game.h>
#include <lfant/FileSystem.h>
#include <lfant/Properties.h>
#include "lfant/Console.h"

// External

namespace lfant
{

Material::Material() :
	texture {Texture::LoadFile("textures/Default.png")},
	shader {Shader::LoadFile("shaders/simple/Diffuse.vert", "shaders/simple/Diffuse.frag")}
{
}

void Material::Load(Properties* prop)
{
	Object::Load(prop);

	string file = prop->Get("texture");
	if(Properties* tex = prop->GetChild("texture"))
	{
		texture = Texture::Load(tex);
	}
	else if(!file.empty())
	{
		texture = Texture::LoadFile(file);
	}

	if(Properties* sh = prop->GetChild("shader"))
	{
		shader = Shader::Load(sh);
	}

//	loaded = true;
}

void Material::Save(Properties *prop) const
{
	Object::Save(prop);
	texture->Save(prop->AddChild("texture"));
	shader->Save(prop->AddChild("shader"));
}

}
