/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-06-18 by Taylor Snead
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

#include "FollowEntity.h"

// Internal
#include <lfant/Transform.h>
#include <lfant/Time.h>
#include <lfant/Game.h>
#include <lfant/Scene.h>

// External

namespace lfant {
namespace galaga {

IMPLEMENT_COMP(FollowEntity)

FollowEntity::FollowEntity()
{
}

FollowEntity::~FollowEntity()
{
}

void FollowEntity::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("x", useX);
	prop->Get("y", useY);
	prop->Get("z", useZ);
	prop->Get("entity", entity);
	prop->Get("speed", speed);
	prop->Get("damping", damping);
}

void FollowEntity::Save(Properties* prop)
{
	Component::Save(prop);

	prop->Set("x", useX);
	prop->Set("y", useY);
	prop->Set("z", useZ);
	prop->Set("entity", entity);
	prop->Set("speed", speed);
	prop->Set("damping", damping);
}

void FollowEntity::Init()
{
	if(entity)
	{

	}
}

void FollowEntity::Update()
{
	if(!entity || (!useX && !useY && !useZ))
	{
		return;
	}

	static vec3 pos = vec3(0);
	pos = ((entity->transform->GetWorldPosition() - owner->transform->GetWorldPosition()) * damping) * (speed * game->time->deltaTime);
	if(!useX)
	{
		pos.x = 0;
	}
	if(!useY)
	{
		pos.y = 0;
	}
	if(!useZ)
	{
		pos.z = 0;
	}
	owner->transform->Translate(pos);
}

}
}