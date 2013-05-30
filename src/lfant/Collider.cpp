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

void Collider::Save(Properties* prop)
{
	Component::Save(prop);

	prop->Set("size", GetSize());
}

void Collider::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("size", size);
}

void Collider::Init()
{
	TriggerEvent("SetCollider", this);
	ConnectEvent(SENDER(owner, SetScale), RECEIVER(this, OnSetScale));
	ConnectEvent(SENDER(owner, SetRigidbody), RECEIVER(this, OnSetRigidbody));
}

void Collider::OnSetRigidbody(Rigidbody* rb)
{
	rigidbody = rb;
}

void Collider::OnSetScale(vec3 scale)
{
	GetShape()->setLocalScaling(vec3_cast<btVector3>(scale));
}

}
