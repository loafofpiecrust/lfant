/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-10-01 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Light.h>

// External

// Internal
#include <lfant/Game.h>
#include <lfant/Renderer.h>

namespace lfant
{

Light::Light()
{
	render = false;
}

Light::~Light()
{

}

void Light::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("color", color);
}

void Light::Save(Properties* prop) const
{
	Component::Save(prop);

	prop->Set("color", color);
}

void Light::Init()
{
	GetGame()->renderer->AddLight(this);
	Renderable::Init();
//	Enable(false);
}

void Light::Update()
{

}

void Light::Render()
{

}

}
