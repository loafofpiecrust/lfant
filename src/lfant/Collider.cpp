/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-11-01 by Taylor Snead
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

#include <lfant/Collider.h>

// External
#include <btBulletCollisionCommon.h>

// Internal
#include <lfant/Rigidbody.h>

namespace lfant
{

void Collider::Init()
{
//	connect( transform, "SetScale", this, &Collider::OnSetScale );
}

void Collider::OnAddComponent(Component* comp)
{
	if(CheckType<Rigidbody*>(comp))
	{
		//	rigidbody = dynamic_cast<Rigidbody*>( comp );
	}
}

btCollisionShape* Collider::GetShape()
{
}

}
