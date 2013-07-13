/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-10-01 by Taylor Snead
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

#include <lfant/Light.h>

// External

// Internal
#include <lfant/Game.h>
#include <lfant/Renderer.h>

namespace lfant
{

Light::Light()
{

}

Light::~Light()
{

}

void Light::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("color", color);
}

void Light::Save(Properties* prop)
{
	Component::Save(prop);

	prop->Set("color", color);
}

void Light::Init()
{
	game->renderer->AddLight(this);
	Renderable::Init();
}

void Light::Update()
{

}

void Light::Render()
{
	
}

}
