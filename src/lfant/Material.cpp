/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-03-18 by Taylor Snead
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

#include <lfant/Material.h>

// Internal
#include <lfant/Game.h>
#include <lfant/FileSystem.h>
#include <lfant/Properties.h>

// External

namespace lfant
{

Material::Material() :
	texture(new Texture),
	shader(new Shader)
{
}

Material::Material(string texture, string shader)
{
	this->texture->LoadFile(texture);
	this->shader->LoadFile(shader);
}

void Material::Load(Properties* prop)
{
	if(Properties* tex = prop->GetChild("texture"))
	{
		texture->Load(tex);
	}

	if(Properties* sh = prop->GetChild("shader"))
	{
		shader->Load(sh);
	}
}

void Material::Save(Properties *prop)
{
	Object::Save(prop);
	texture->Save(prop->AddChild());
	shader->Save(prop->AddChild());
}

}
