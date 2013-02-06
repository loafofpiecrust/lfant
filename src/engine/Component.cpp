/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-07-20 by Taylor Snead
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

#include "Component.hpp"

// External

// Internal
#include "Entity.hpp"

using namespace sfs;

Component::Component( Entity* owner ) :
	owner( owner ), transform( owner->transform )
{
}

Component::~Component()
{
}

/*******************************************************************************
 *		General functions
 *		@area General
 *******************************************************************************/

void Component::Destroy()
{
	OnDestroy();
	owner->RemoveComponent( this );
	delete this;
}

/*******************************************************************************
 *		Get/Set Properties functions
 *		@area Properties
 *******************************************************************************/

void Component::SetEnabled(bool enable)
{
	_enabled = enable;
	if( _enabled )
	{
		OnEnable();
	}
	else
	{
		OnDisable();
	}
}

bool Component::GetEnabled()
{
	return _enabled;
}
